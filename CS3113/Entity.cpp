#include "Entity.h"

Entity::Entity() :
    mPosition{ 0.0f, 0.0f }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ DEFAULT_SIZE, DEFAULT_SIZE },
    mColliderDimensions{ DEFAULT_SIZE, DEFAULT_SIZE },
    mTexture{}, mTextureType{ SINGLE }, mSpriteSheetDimensions{},
    mAnimationAtlas{}, mAnimationIndices{}, mDirection{ RIGHT },
    mFrameSpeed{ 0 }, mSpeed{ DEFAULT_SPEED }, mAngle{ 0.0f },
    mEntityType{ NONE }, mAIType{ WANDERER }, mAIState{ IDLE } {}

Entity::Entity(
    Vector2     position,
    Vector2     scale,
    const char *textureFilepath,
    EntityType  entityType
) :
    mPosition{ position }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ scale }, mColliderDimensions{ scale },
    mTexture{ LoadTexture(textureFilepath) }, mTextureType{ SINGLE },
    mSpriteSheetDimensions{}, mAnimationAtlas{}, mAnimationIndices{},
    mDirection{ RIGHT }, mFrameSpeed{ 0 }, mSpeed{ DEFAULT_SPEED },
    mAngle{ 0.0f }, mEntityType{ entityType }, mAIType{ WANDERER },
    mAIState{ IDLE } {}

Entity::Entity(
    Vector2                               position,
    Vector2                               scale,
    const char                           *textureFilepath,
    TextureType                           textureType,
    Vector2                               spriteSheetDimensions,
    std::map<Direction, std::vector<int>> animationAtlas,
    EntityType                            entityType
) :
    mPosition{ position }, mMovement{ 0.0f, 0.0f },
    mVelocity{ 0.0f, 0.0f }, mAcceleration{ 0.0f, 0.0f },
    mScale{ scale }, mColliderDimensions{ scale },
    mTexture{ LoadTexture(textureFilepath) }, mTextureType{ textureType },
    mSpriteSheetDimensions{ spriteSheetDimensions },
    mAnimationAtlas{ animationAtlas },
    mAnimationIndices{ animationAtlas.at(RIGHT) }, mDirection{ RIGHT },
    mFrameSpeed{ DEFAULT_FRAME_SPEED }, mSpeed{ DEFAULT_SPEED },
    mAngle{ 0.0f }, mEntityType{ entityType }, mAIType{ WANDERER },
    mAIState{ IDLE } {}

Entity::~Entity() { UnloadTexture(mTexture); }

void Entity::checkCollisionY(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (mVelocity.y > 0)
            {
                mPosition.y        -= yOverlap;
                mVelocity.y         = 0;
                mIsCollidingBottom  = true;
            }
            else if (mVelocity.y < 0)
            {
                mPosition.y     += yOverlap;
                mVelocity.y      = 0;
                mIsCollidingTop  = true;
            }
        }
    }
}

void Entity::checkCollisionX(
    Entity *collidableEntities, int collisionCheckCount
)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(
                yDistance - (mColliderDimensions.y / 2.0f) -
                (collidableEntity->mColliderDimensions.y / 2.0f)
            );

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(
                xDistance - (mColliderDimensions.x / 2.0f) -
                (collidableEntity->mColliderDimensions.x / 2.0f)
            );

            if (mVelocity.x > 0)
            {
                mPosition.x       -= xOverlap;
                mVelocity.x        = 0;
                mIsCollidingRight  = true;
            }
            else if (mVelocity.x < 0)
            {
                mPosition.x      += xOverlap;
                mVelocity.x       = 0;
                mIsCollidingLeft  = true;
            }
        }
    }
}

void Entity::checkCollisionY(Map *map)
{
    if (map == nullptr) return;

    Vector2 topCentreProbe = {
        mPosition.x, mPosition.y - (mColliderDimensions.y / 2.0f)
    };
    Vector2 topLeftProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f),
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };
    Vector2 topRightProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f),
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };

    Vector2 bottomCentreProbe = {
        mPosition.x, mPosition.y + (mColliderDimensions.y / 2.0f)
    };
    Vector2 bottomLeftProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f),
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };
    Vector2 bottomRightProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f),
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(topCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topLeftProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(topRightProbe, &xOverlap, &yOverlap)) &&
        mVelocity.y < 0.0f)
    {
        mPosition.y     += yOverlap;
        mVelocity.y      = 0.0f;
        mIsCollidingTop  = true;
    }

    if ((map->isSolidTileAt(bottomCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomLeftProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(bottomRightProbe, &xOverlap, &yOverlap)) &&
        mVelocity.y > 0.0f)
    {
        mPosition.y        -= yOverlap;
        mVelocity.y         = 0.0f;
        mIsCollidingBottom  = true;
    }
}

void Entity::checkCollisionX(Map *map)
{
    if (map == nullptr) return;

    Vector2 leftCentreProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f), mPosition.y
    };
    Vector2 leftTopProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f),
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };
    Vector2 leftBottomProbe = {
        mPosition.x - (mColliderDimensions.x / 2.0f),
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };

    Vector2 rightCentreProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f), mPosition.y
    };
    Vector2 rightTopProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f),
        mPosition.y - (mColliderDimensions.y / 2.0f)
    };
    Vector2 rightBottomProbe = {
        mPosition.x + (mColliderDimensions.x / 2.0f),
        mPosition.y + (mColliderDimensions.y / 2.0f)
    };

    float xOverlap = 0.0f;
    float yOverlap = 0.0f;

    if ((map->isSolidTileAt(rightCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(rightTopProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(rightBottomProbe, &xOverlap, &yOverlap)) &&
        mVelocity.x > 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x       -= xOverlap * 1.01f;
        mVelocity.x        = 0.0f;
        mIsCollidingRight  = true;
    }

    if ((map->isSolidTileAt(leftCentreProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(leftTopProbe, &xOverlap, &yOverlap) ||
         map->isSolidTileAt(leftBottomProbe, &xOverlap, &yOverlap)) &&
        mVelocity.x < 0.0f && yOverlap >= 0.5f)
    {
        mPosition.x      += xOverlap * 1.01f;
        mVelocity.x       = 0.0f;
        mIsCollidingLeft  = true;
    }
}

bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive() || other == this) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    mAnimationTime += deltaTime;
    float secondsPerFrame = 1.0f / mFrameSpeed;

    if (mAnimationTime >= secondsPerFrame)
    {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

void Entity::displayCollider()
{
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x,
        colliderBox.y,
        colliderBox.width,
        colliderBox.height,
        GREEN
    );
}

void Entity::AIWander()
{
    if (mIsCollidingLeft)       moveRight();
    else if (mIsCollidingRight) moveLeft();
    else if (mDirection == LEFT) moveLeft();
    else                         moveRight();
}

void Entity::AIFollow(Entity *target)
{
    switch (mAIState)
    {
    case IDLE:
        if (Vector2Distance(mPosition, target->getPosition()) < 400.0f)
            mAIState = WALKING;
        break;
    case WALKING:
        if (mPosition.x > target->getPosition().x)
            moveLeft();
        else
            moveRight();
        break;
    default:
        break;
    }
}

void Entity::AILerp(Entity *target, float deltaTime)
{
    switch (mAIState)
    {
    case IDLE:
        if (Vector2Distance(mPosition, target->getPosition()) < 350.0f)
            mAIState = FOLLOWING;
        break;
    case FOLLOWING:
        mPosition.x += (target->getPosition().x - mPosition.x) * mLerpFactor * deltaTime;
        mPosition.y += (target->getPosition().y - mPosition.y) * mLerpFactor * deltaTime;
        break;
    default:
        break;
    }
}

void Entity::AIActivate(Entity *target, float deltaTime)
{
    switch (mAIType)
    {
    case WANDERER:
        AIWander();
        break;
    case FOLLOWER:
        AIFollow(target);
        break;
    case LERPER:
        AILerp(target, deltaTime);
        break;
    default:
        break;
    }
}

void Entity::update(
    float   deltaTime,
    Entity *player,
    Map    *map,
    Entity *collidableEntities,
    int     collisionCheckCount
)
{
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == NPC) AIActivate(player, deltaTime);

    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    if (mIsJumping)
    {
        mIsJumping = false;

        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidableEntities, collisionCheckCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidableEntities, collisionCheckCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS &&
        (GetLength(mMovement) != 0 || mEntityType == NPC))
        animate(deltaTime);
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
    case SINGLE:
        textureArea = {
            0.0f, 0.0f,
            static_cast<float>(mTexture.width),
            static_cast<float>(mTexture.height)
        };
        break;
    case ATLAS:
        textureArea = getUVRectangle(
            &mTexture,
            mAnimationIndices[mCurrentFrameIndex],
            mSpriteSheetDimensions.x,
            mSpriteSheetDimensions.y
        );
        break;
    default:
        break;
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );
}
