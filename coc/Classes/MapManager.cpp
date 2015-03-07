#include "Base.h"
#include "MapManager.h"

const float MAP_MOVE_SPEED = 20.0f;
const float MAP_BORDER_MARGIN = 50.0f;
const float MAP_MIN_SCALE = 0.3f;
const float MAP_MAX_SCALE = 1.0f;

static std::map<TileMapLayerType, std::string> s_tileMapLayerTypeToString = {
    { TileMapLayerType::BackgroundLayer, "backgroundLayer" },
    { TileMapLayerType::GameObjcetLayer, "gameObjectLayer" },
};


MapManager::~MapManager()
{
    Director::getInstance()->setProjection(Director::Projection::DEFAULT);
    Director::getInstance()->setDepthTest(false);
}


bool MapManager::init(Node* parentNode, const std::string& titleMapFileName)
{
    auto glView = Director::getInstance()->getOpenGLView();
    auto windowHandle = glView->getWin32Window();
    GetWindowRect(windowHandle, &_clientRect);

    _tileMap = cocos2d::experimental::TMXTiledMap::create(titleMapFileName);
    _tileMap->setScale(_mapScale);
    parentNode->addChild(_tileMap);

    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(true);

    //resolveMapShakeWhenMove();

    return true;
}

void MapManager::updateMapPosition()
{
    auto titleMapPosition = _tileMap->getPosition();
    auto titleMapSize = _tileMap->getContentSize() * _tileMap->getScale();

    if (_cursorPoint.x <= _clientRect.left + MAP_BORDER_MARGIN)
    {
        titleMapPosition.x += MAP_MOVE_SPEED;
    }

    if (_cursorPoint.y <= _clientRect.top + MAP_BORDER_MARGIN)
    {
        titleMapPosition.y += MAP_MOVE_SPEED;
    }

    if (_cursorPoint.x >= _clientRect.right - MAP_BORDER_MARGIN)
    {
        titleMapPosition.x -= MAP_MOVE_SPEED;
    }

    if (_cursorPoint.y >= _clientRect.bottom - MAP_BORDER_MARGIN)
    {
        titleMapPosition.y -= MAP_MOVE_SPEED;
    }

    if (titleMapPosition.x > 0.0f)
    {
        titleMapPosition.x = 0.0f;
    }

    auto clientWidth = _clientRect.right - _clientRect.left;
    auto clientHeight = _clientRect.bottom - _clientRect.top;
    auto mapMinXPosition = clientWidth - titleMapSize.width;
    auto mapMinYPosition = clientHeight - titleMapSize.height;
    titleMapPosition.x = std::max(titleMapPosition.x, mapMinXPosition);
    titleMapPosition.y = std::max(titleMapPosition.y, mapMinYPosition);
    titleMapPosition.x = std::min(titleMapPosition.x, 0.0f);
    titleMapPosition.y = std::min(titleMapPosition.y, 0.0f);

    _tileMap->setPosition(titleMapPosition);
}

void MapManager::updateMapScale(Event* event)
{
    auto lastMapScale = _mapScale;

    auto mouseEvent = static_cast<EventMouse*>(event);
    if (mouseEvent->getScrollY() == 1)
    {
        _mapScale = std::max(_mapScale - 0.1f, MAP_MIN_SCALE);
    }
    else
    {
        _mapScale = std::min(_mapScale + 0.1f, MAP_MAX_SCALE);
    }

    auto tileMapContentSize = _tileMap->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    if (tileMapContentSize.width * _mapScale < visibleSize.width || tileMapContentSize.height * _mapScale < visibleSize.height)
    {
        return;
    }

    auto cursorPointInMap = _tileMap->convertToNodeSpace(_cursorPoint);
    auto cursorScalePointInMap = cursorPointInMap * _mapScale;
    auto moveDelta = cursorPointInMap * lastMapScale - cursorScalePointInMap;

    auto tileMapPosition = _tileMap->getPosition();
    _tileMap->setPosition(tileMapPosition + moveDelta);
    _tileMap->setScale(_mapScale);
}

void MapManager::syncCursorPoint(const Vec2& cursorPoint)
{
    _cursorPoint = cursorPoint;
}

void MapManager::resolveMapShakeWhenMove()
{
    auto& children = _tileMap->getChildren();
    for (auto& child : children)
    {
        auto tile = static_cast<Sprite*>(child);
        auto tileTexture = tile->getTexture();
        if (tileTexture)
        {
            tileTexture->setAntiAliasTexParameters();
        }
    }
}

Point MapManager::getTileSubscript()
{
    Point tileSubscript;

    auto mapSize = _tileMap->getMapSize();
    auto tileSize = _tileMap->getTileSize();
    auto positionInTileMap = _tileMap->convertToNodeSpace(_cursorPoint);
    tileSubscript.x = (mapSize.height - (positionInTileMap.y / tileSize.height)) + ((positionInTileMap.x / tileSize.width) - mapSize.width / 2);
    tileSubscript.y = (mapSize.height - (positionInTileMap.y / tileSize.height)) - ((positionInTileMap.x / tileSize.width) - mapSize.width / 2);

    tileSubscript.x = std::max(0.0f, tileSubscript.x);
    tileSubscript.x = std::min(mapSize.width - 1.0f, tileSubscript.x);
    tileSubscript.y = std::max(0.0f, tileSubscript.y);
    tileSubscript.y = std::min(mapSize.height - 1.0f, tileSubscript.y);

    tileSubscript.x = std::floor(tileSubscript.x);
    tileSubscript.y = std::floor(tileSubscript.y);

    return tileSubscript;
}

DebugInfo MapManager::getDebugInfo(TileMapLayerType tileMapLayerType)
{
    DebugInfo tileDebugInfo;

    auto currentTileMapLayer = _tileMap->getLayer(s_tileMapLayerTypeToString[tileMapLayerType]);
    auto tileSubscript = getTileSubscript();

    tileDebugInfo.gid = currentTileMapLayer->getTileGIDAt(tileSubscript);
    tileDebugInfo.tileMapLayerName = s_tileMapLayerTypeToString[tileMapLayerType];
    tileDebugInfo.cursorPoint = _cursorPoint;
    tileDebugInfo.tileSubscript = tileSubscript;

    return tileDebugInfo;
}

cocos2d::Vec2 MapManager::convertCursorPositionToTileMapSpace()
{
    return _tileMap->convertToNodeSpace(_cursorPoint);
}

void MapManager::addChildInGameObjectLayer(Node* gameObject)
{
    auto gameObjectLayer = _tileMap->getLayer("gameObjectLayer");
    gameObjectLayer->addChild(gameObject, 1);
}
