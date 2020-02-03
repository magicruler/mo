#include "ray_cast.h"
#include "game.h"
#include "scene.h"
#include "aabb.h"
#include "mathematica.h"

namespace Physics
{
    bool IntersectRay(Ray const& r, AABB& aabb, float& t)  
    {
        glm::vec3 pmin = aabb.minPos;
        glm::vec3 pmax = aabb.maxPos;

        if ((pmax.x - pmin.x) >= 0.0f)
        {
            return false;
        }

        if ((pmax.y - pmin.y) >= 0.0f)
        {
            return false;
        }

        if ((pmax.z - pmin.z) >= 0.0f)
        {
            return false;
        }

        float tmin, tmax, tymin, tymax, tzmin, tzmax;

        glm::vec3 invertDir = glm::vec3(1.0f, 1.0f, 1.0f) / r.direction;
        int sign0 = (invertDir.x < 0);
        int sign1 = (invertDir.y < 0);
        int sign2 = (invertDir.z < 0);

        auto starThis = aabb;

        tmin = (starThis[sign0].x - r.origin.x) * invertDir.x;

        tmax = (starThis[1 - sign0].x - r.origin.x) * invertDir.x;
        tymin = (starThis[sign1].y - r.origin.y) * invertDir.y;
        tymax = (starThis[1 - sign1].y - r.origin.y) * invertDir.y;

        if ((tmin > tymax) || (tymin > tmax))
        {
            return false;
        }

        if (tymin > tmin)
        {
            tmin = tymin;
        }

        if (tymax < tmax)
        {
            tmax = tymax;
        }

        tzmin = (starThis[sign2].z - r.origin.z) * invertDir.z;
        tzmax = (starThis[1 - sign2].z - r.origin.z) * invertDir.z;

        if ((tmin > tzmax) || (tzmin > tmax))
        {
            return false;
        }

        if (tzmin > tmin)
        {
            tmin = tzmin;
        }

        if (tzmax < tmax)
        {
            tmax = tzmax;
        }

        t = Math::Min(tmin, tmax);

        return true;
    }

	void RayCast(Ray ray, LAYER_MASK layerMask, RayCastInteraction& interaction)
	{
		std::vector<Actor*> sceneFrontLayer;
		std::vector<Actor*> sceneBackLayer;

		Scene* activeScene = Game::ActiveSceneGetPointer();

		sceneFrontLayer = activeScene->rootNode->GetChildren();

        float minT = INFINITY;
        Actor* targetActor = nullptr;

		while (sceneFrontLayer.size() != 0)
		{
			sceneBackLayer.clear();

			for (int i = 0; i < sceneFrontLayer.size(); i++)
			{
				auto actor = sceneFrontLayer[i];
				AABB actorBoundingBox = actor->GetAABB();

                float t = 0.0f;
                if (IntersectRay(ray, actorBoundingBox, t))
                {
                    // actor could only have one flag
                    if ((t < minT) && (actor->GetLayerFlag() & layerMask))
                    {
                        minT = t;
                        targetActor = actor;
                    }
                }

				for (int j = 0; j < actor->GetChildren().size(); j++)
				{
					sceneBackLayer.push_back(actor->GetChildren()[j]);
				}
			}

			sceneFrontLayer.swap(sceneBackLayer);
		}

        if (targetActor != nullptr)
        {
            interaction.target = targetActor;
            interaction.distance = minT;
            interaction.position = ray.origin + ray.direction * minT;
        }
	}
};