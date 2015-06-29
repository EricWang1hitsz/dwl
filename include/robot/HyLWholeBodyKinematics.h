#ifndef DWL_HyLWholeBodyKinematics_H
#define DWL_HyLWholeBodyKinematics_H

#include <model/WholeBodyKinematics.h>
#include <iit/robots/hyl/jacobians.h>
#include <iit/robots/hyl/transforms.h>


namespace dwl
{

namespace robot
{

class HyLWholeBodyKinematics : public model::WholeBodyKinematics
{
	public:
		HyLWholeBodyKinematics();
		~HyLWholeBodyKinematics();

		void init();
		void updateState(const iit::rbd::Vector6D& base_pos, const Eigen::VectorXd& joint_pos);

	private:
		iit::HyL::Jacobians jacs_;
		iit::HyL::HomogeneousTransforms hom_tf_;
		iit::HyL::MotionTransforms motion_tf_;
};

} //@namespace robot
} //@namespace dwl

#endif