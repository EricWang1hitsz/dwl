#include <dwl/simulation/LinearControlledSlipModel.h>


namespace dwl
{

namespace simulation
{

LinearControlledSlipModel::LinearControlledSlipModel() : init_model_(false),
		init_response_(false), initial_time_(0.), initial_length_(0.),
		slip_omega_(0.), spring_omega_(0.), d_1_(0.), d_2_(0.)
{

}


LinearControlledSlipModel::~LinearControlledSlipModel()
{

}


void LinearControlledSlipModel::setModelProperties(SlipProperties model)
{
	slip_ = model;
	init_model_ = true;
}


void LinearControlledSlipModel::initResponse(const ReducedBodyState& state,
											 const SlipControlParams& params)
{
	if (!init_model_) {
		printf(YELLOW "Warning: could not initialized the initResponse because"
				" there is not defined the SLIP model\n" COLOR_RESET);
		return;
	}

	// Saving the initial state
	initial_state_ = state;

	// Saving the SLIP control params
	params_ = params;

	// Computing the coefficients of the Spring Loaded Inverted Pendulum
	// (SLIP) response
	double slip_height = initial_state_.com_pos(rbd::Z) - initial_state_.cop(rbd::Z);
	slip_omega_ = sqrt(slip_.gravity / slip_height);
	double alpha = 2 * slip_omega_ * params_.duration;
	Eigen::Vector2d slip_hor_proj = (initial_state_.com_pos - initial_state_.cop).head<2>();
	Eigen::Vector2d slip_hor_disp = initial_state_.com_vel.head<2>() * params_.duration;
	beta_1_ = slip_hor_proj / 2 +
			(slip_hor_disp - params_.cop_shift.head<2>()) / alpha;
	beta_2_ = slip_hor_proj / 2 -
			(slip_hor_disp - params_.cop_shift.head<2>()) / alpha;

	// Computing the initial length of the pendulum
	initial_length_ = (initial_state_.com_pos - initial_state_.cop).norm();

	// Computing the coefficients of the spring-mass system response
	spring_omega_ = sqrt(slip_.stiffness / slip_.mass);
	d_1_ = initial_state_.com_pos(rbd::Z) - initial_length_ +
			slip_.gravity /	pow(spring_omega_,2);
	d_2_ = initial_state_.com_vel(rbd::Z) / spring_omega_ -
			params_.length_shift / (spring_omega_ * params_.duration);

	init_response_ = true;
}


void LinearControlledSlipModel::computeResponse(ReducedBodyState& state,
												double time)
{
	if (!init_response_) {
		printf(YELLOW "Warning: could not be computed the SLIP response because."
				" You should call initResponse\n" COLOR_RESET);
		return;
	}

	// Checking the preview duration
	if (time < initial_time_)
		return; // duration it's always positive, and makes sense when
				// is bigger than the sample time


	// Computing the delta time w.r.t. the initial time
	double dt = time - initial_time_;
	state.time = time;

	// Computing the horizontal motion of the CoM according to
	// the SLIP system
	state.com_pos.head<2>() =
			beta_1_ * exp(slip_omega_ * dt) +
			beta_2_ * exp(-slip_omega_ * dt) +
			(params_.cop_shift.head<2>() / params_.duration) * dt +
			initial_state_.cop.head<2>();
	state.com_vel.head<2>() =
			beta_1_ * slip_omega_ * exp(slip_omega_ * dt) -
			beta_2_ * slip_omega_ * exp(-slip_omega_ * dt) +
			params_.cop_shift.head<2>() / params_.duration;
	state.com_vel.head<2>() =
			beta_1_ * pow(slip_omega_,2) * exp(slip_omega_ * dt) +
			beta_2_ * pow(slip_omega_,2) * exp(-slip_omega_ * dt);

	// Computing the vertical motion of the CoM according to
	// the spring-mass system
	state.com_pos(rbd::Z) =
			d_1_ * cos(spring_omega_ * dt) +
			d_2_ * sin(spring_omega_ * dt) +
			(params_.length_shift / params_.duration) * dt +
			initial_length_ - slip_.gravity / pow(spring_omega_,2);
	state.com_vel(rbd::Z) =
			-d_1_ * spring_omega_ * sin(spring_omega_ * dt) +
			d_2_ * spring_omega_ * cos(spring_omega_ * dt) +
			params_.length_shift / params_.duration;
	state.com_acc(rbd::Z) =
			-d_1_ * pow(spring_omega_,2) * cos(spring_omega_ * dt) -
			d_2_ * pow(spring_omega_,2) * sin(spring_omega_ * dt);
	state.com_pos(rbd::Z) = initial_state_.com_pos(rbd::Z);//TODO for debugging

	// Computing the CoP position given the linear assumption
	state.cop = initial_state_.cop + (dt / params_.duration) * params_.cop_shift;
}

} //@namespace simulation
} //@namespace dwl