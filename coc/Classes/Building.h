#pragma once

enum class BuildingStatus
{
    Invalid = 0,

    PrepareToBuild,
    BeingBuilt,
    Working,
    Destory,

    Total
};

class Building : public GameObject
{
public:
    static Building* create(ForceType forceType, const string& buildingTemplateName, const Vec2& position, int uniqueID);

    bool isReadyToRemove() override;
    void clearDebugDraw() override;

    void update(float delta) override;
    void updateStatus(BuildingStatus buildingStatus);
    BuildingStatus getBuildingStatus();

    bool canUpdateToWorkingStatus();
private:
    bool init(ForceType forceType, const string& buildingTemplateName, const Vec2& position, int uniqueID);

    void initBuildingStatusSprites(const string& buildingTemplateName);
    Sprite* createBuildingStatusSprite(const string& buildingTemplateName, BuildingStatus buildingStatus, int opacity = 255);

    void initBottomGridSprites(const string& buildingTemplateName);
    void initHPBar();
    void initBattleData(const string& buildingTemplateName);
    void clear();

    void onPrepareToRemove() override;
    void debugDraw() override;

    void followCursorInPrepareToBuildStatus();
    void updateBottomGridTextureInPrepareToBuildStatus();

    map<BuildingStatus, Sprite*> _buildingStatusSpriteMap;
    vector<Sprite*> _bottomGridSpritesList;
    Vec2 _bottomGridsPlaneCenterPositionInLocalSpace; // ������ײ���Ƭ������λ��

    BuildingStatus _buildingStatus;
    bool _canUpdateToWorkingStatus = true;
};