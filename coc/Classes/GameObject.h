#pragma once

enum class GameObjectType
{
    Invalid,

    Npc,
    DefenceInBuildingNpc,
    Building,
};

enum class ForceType
{
    Invalid,

    Player,
    AI,
};

enum class BulletType
{
    Invalid,

    Arrow,
    Magic,
    Bomb,
};

enum class DamageType
{
    Invalid,

    AreaOfEffect,
    Normal,
};

const int ENEMY_UNIQUE_ID_INVALID = 0;
const int GAME_OBJECT_UNIQUE_ID_INVALID = 0; 
const string HP_BAR_BACKGROUND_TEXTURE_NAME = "HPBarBackground.png";
const int TEAM_INVALID_ID = -1;
const float FORBID_ENEMY_APPROACH_TIME_INTERVAL = 15.0f;
const int MAX_LEVEL = 5;

const string PLAYER_HP_BAR_TEXTURE_NAME = "PlayerHPBar.png";
const string AI_HP_BAR_TEXTURE_NAME = "AIHPBar.png";

class GameObject : public Sprite
{
public:
    virtual ~GameObject();
    int getUniqueID();
    void depthSort(const Size& tileSize);

    virtual void setSelected(bool isSelect);
    bool isSelected();
    virtual bool isReadyToRemove() = 0;

    GameObjectType getGameObjectType();
    ForceType getForceType();
    int getAttackPower();
    float getExtraEnemyAttackRadius();
    DamageType getDamageType();
    float getAoeDamageRadius();

    void showHPBar();
    void hideHPBar();
    virtual void costHP(int costHPAmount);
    void addHP(int addHPAmount);
    void updateHP();

    virtual void setEnemyUniqueID(int uniqueID);
    virtual int getEnemyUniqueID();

    void update(float delta) override;
    virtual void clearDebugDraw();

    float getCollisionRadius();

    int getTeamID();
    void setTeamID(int teamID);

    const string& getTemplateName();

    bool canEnemyApproach();
    void launchForbidEnemyApproachTimer();

    void onAddEnemyTechnologyPoint();

    void upgrade();
    void upgradePropertyBy(int level);
protected:
    GameObject();
    bool init() override;
    virtual void debugDraw() = 0;
    virtual void onPrepareToRemove() = 0;

    void onTechnologyPointLabelRemove(Node* label);

    virtual void updateLevelRepresentTexture(const string& spriteFrameName) = 0;

    int _hp = 0;
    int _maxHp = 0;
    ui::LoadingBar* _hpBar = nullptr;
   
    int _attackPower = 0;
    int _maxAttackRadius = 0;
    float _extraEnemyAttackRadius = 0.0f;   // �з�GameObject������Χ�ӳ�
    BulletType _bulletType = BulletType::Invalid;
    DamageType _damageType = DamageType::Invalid;
    float _aoeDamageRadius = 0.0f;

    int _level = 0;
    int _maxLevel = 0;

    int _enemyUniqueID = ENEMY_UNIQUE_ID_INVALID;
    int _uniqueID = 0;

    bool _isSelected = false;

    GameObjectType _gameObjectType = GameObjectType::Invalid;
    ForceType _forceType = ForceType::Invalid;

    DrawNode* _debugDrawNode = nullptr;
    Sprite* _selectedTips = nullptr;
    Label* _teamIDLabel = nullptr;
    int _teamID = TEAM_INVALID_ID;

    float _showHPBarTotalTimeAfterBeingAttacked = 0.0f;

    float _collisionRadius = 0.0f;

    string _templateName;

    float _forbidEnemyApproachTime = 0.0f;

    int _technologyPointForEnemy = 0;
};

class GameObjectFactory
{
public:
    static GameObject* create(GameObjectType gameObjectType, ForceType forceType, const string& jobName, const Vec2& position, int level);
};