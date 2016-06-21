#ifndef DWL__ENVIRONMENT__ENVIRONMENT_INFORMATION__H
#define DWL__ENVIRONMENT__ENVIRONMENT_INFORMATION__H

#include <dwl/environment/SpaceDiscretization.h>
#include <dwl/utils/utils.h>


namespace dwl
{

namespace environment
{

/**
 * @class TerrainMap
 * @brief Class for defining the terrain information
 */
class TerrainMap
{
	public:
		/** @brief Constructor function */
		TerrainMap();

		/** @brief Destructor function */
		~TerrainMap();

		/**
		 * @brief Sets the terrain cost-map and height-map
		 * @param std::vector<RewardCell> Reward map
		 */
		void setRewardMap(std::vector<RewardCell> reward_map);

		/**
		 * @brief Sets the obstacle map
		 * @param std::vector<Cell> Obstacle map
		 */
		void setObstacleMap(std::vector<Cell> obstacle_map);

		/**
		 * @brief Sets the terrain resolution (cost and height map) of the plane or height
		 * @param double Resolution value
		 * @param bool Indicates if the key represents a plane or a height
		 */
		void setTerrainResolution(double resolution, bool plane);

		/**
		 * @brief Sets the obstacle resolution of the plane or height
		 * @param double Resolution value
		 * @param bool Indicates if the key represents a plane or a height
		 */
		void setObstacleResolution(double resolution, bool plane);

		/**
		 * @brief Sets the state resolution of the plane or height
		 * @param double Position resolution value
		 * @param double Angular resolution value
		 */
		void setStateResolution(double position_resolution, double angular_resolution);

		/**
		 * @brief Gets the terrain cost-map (using vertex id)
		 * @param CostMap& Cost map of the terrain
		 */
		void getTerrainCostMap(CostMap& costmap);

		/**
		 * @brief Gets the terrain height-map (using vertex id)
		 * @param HeightMap& Height map of the terrain
		 */
		void getTerrainHeightMap(HeightMap& heightmap);

		/**
		 * @brief Gets the obstacle-map (using vertex id)
		 * @param ObstacleMap& Obstacle map of the terrain
		 */
		void getObstacleMap(ObstacleMap& obstaclemap);

		/** @brief Gets the terrain resolution (cost and height map) */
		double getTerrainResolution();

		/** @brief Gets the obstacle resolution */
		double getObstacleResolution();

		/**
		 * @brief Gets the average cost of the terrain
		 * @return The average cost of the terrain
		 */
		double getAverageCostOfTerrain();

		/**
		 * @brief Gets the terrain discrete model of the space according
		 * the resolution of the terrain
		 * @return The discrete space model
		 */
		SpaceDiscretization& getTerrainSpaceModel();

		/**
		 * @brief Gets the obstacle discrete model of the space according
		 * the resolution of the reward map
		 * @return The discrete space model
		 */
		SpaceDiscretization& getObstacleSpaceModel();

		/**
		 * @brief Indicates if it was defined terrain information
		 * @return True if it was defined terrain cost information, otherwise false
		 */
		bool isTerrainInformation();

		/**
		 * @brief Indicates if it was defined terrain obstacle information
		 * @return True if it was defined terrain obstacle information, otherwise false
		 */
		bool isObstacleInformation();


	private:
		/**
		 *  @brief Object of the SpaceDiscretization class for defining the
		 *  conversion routines for the terrain cost-map */
		environment::SpaceDiscretization terrain_discretization_;

		/**
		 * @brief Object of the SpaceDiscretization class for defining the
		 * conversion routines for the terrain obstacle-map */
		environment::SpaceDiscretization obstacle_discretization_;

		/** @brief Gathers the terrain cost values that are mapped using the
		 * vertex id */
		CostMap costmap_;

		/** @brief Gathers the obstacles that are mapped using the vertex id */
		ObstacleMap obstaclemap_;

		/** @brief Gathers the height values that are mapped using the vertex id */
		HeightMap heightmap_;

		/** @brief Average terrain cost which is used for unknown areas */
		double average_cost_;

		/** @brief Indicates if it was defined terrain information */
		bool terrain_information_;

		/** @brief Indicates if it was defined obstacle information */
		bool obstacle_information_;

		/** @brief Terrain resolution (cost and height map) of the environment */
		double terrain_resolution_;

		/** @brief Obstacle resolution of the environment */
		double obstacle_resolution_;
};

} //@namespace environment
} //@namespace dwl

#endif
