#include "Base.h"
#include "GameObjectSelectBox.h"

void GameObjectSelectBox::setMouseDownStatus(bool isMouseDown)
{
    _isMouseDown = isMouseDown;
}

void GameObjectSelectBox::setMouseDownPoint(const Vec2& point)
{
    _mouseDownPoint = point;
}

void GameObjectSelectBox::syncCursorPoint(const Vec2& point)
{
    _cursorPoint = point;
}

void GameObjectSelectBox::update(float delta)
{
    clear();

    if (_isMouseDown)
    {
        _leftBottomPoint.x = std::min(_mouseDownPoint.x, _cursorPoint.x);
        _leftBottomPoint.y = std::min(_mouseDownPoint.y, _cursorPoint.y);

        _boxSize.width = std::abs(_mouseDownPoint.x - _cursorPoint.x);
        _boxSize.height = std::abs(_mouseDownPoint.y - _cursorPoint.y);

        _rightTopPoint.x = _leftBottomPoint.x + _boxSize.width;
        _rightTopPoint.y = _leftBottomPoint.y + _boxSize.height;

        drawSolidRect(_leftBottomPoint, _rightTopPoint, _color);

        _rect.setRect(_leftBottomPoint.x, _leftBottomPoint.y, _boxSize.width, _boxSize.height);
    }
}

bool GameObjectSelectBox::init()
{
    if (!DrawNode::init())
    {
        return false;
    }

    _color = Color4F(0.0f, 0.0f, 1.0f, 0.2f);

    setPositionZ(MAX_GAME_OBJECT_COUNT);

    scheduleUpdate();

    return true;
}

Rect GameObjectSelectBox::getRect()
{
    return _rect;
}
