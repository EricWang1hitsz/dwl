#include <planning/ContactPlanning.h>


namespace dwl
{

namespace planning
{

ContactPlanning::ContactPlanning() : environment_(NULL), robot_(NULL), computation_time_(std::numeric_limits<double>::max()), contact_horizon_(0)
{

}


ContactPlanning::~ContactPlanning()
{

}


void ContactPlanning::reset(robot::Robot* robot, environment::EnvironmentInformation* environment)
{
	printf(BLUE "Setting the robot properties in the contact planner \n" COLOR_RESET);
	robot_ = robot;

	printf(BLUE "Setting the environment information in the contact planner \n" COLOR_RESET);
	environment_ = environment;

	for (int i = 0; i < features_.size(); i++)
		features_[i]->reset(robot);
}


void ContactPlanning::addFeature(environment::Feature* feature)
{
	double weight;
	feature->getWeight(weight);
	printf(GREEN "Adding the %s feature with a weight of %f to the contact planner\n" COLOR_RESET, feature->getName().c_str(), weight);
	features_.push_back(feature);
}


void ContactPlanning::setComputationTime(double computation_time)
{
	printf("Setting the allowed computation time of the contact solver to %f \n", computation_time);
	computation_time_ = computation_time;
}


void ContactPlanning::setContactHorizon(int horizon)
{
	contact_horizon_ = horizon;
}

} //@namespace planning
} //@namespace dwl