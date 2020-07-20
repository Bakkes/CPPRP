#include "CPPBM.h"
#include <math.h>

BAKKESMOD_PLUGIN(CPPBM, "CPPBM", "0.1", 0);

Quat normalize(Quat q) {
	double n = sqrt(q.X*q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
	q.X /= n;
	q.Y /= n;
	q.Z /= n;
	q.W /= n;
	return q;
}

Quat minusQuat(Quat v0, Quat v1)
{
	return { v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z, v0.W - v1.W };
}

Quat mulQuat(Quat lhs, Quat rhs)
{
	return { (lhs.W * rhs.X) + (lhs.X * rhs.W) + (lhs.Y * rhs.Z) - (lhs.Z * rhs.Y),
		(lhs.W * rhs.Y) + (lhs.Y * rhs.W) + (lhs.Z * rhs.X) - (lhs.X * rhs.Z),
		(lhs.W * rhs.Z) + (lhs.Z * rhs.W) + (lhs.X * rhs.Y) - (lhs.Y * rhs.X),
		(lhs.W * rhs.W) - (lhs.X * rhs.X) - (lhs.Y * rhs.Y) - (lhs.Z * rhs.Z) };
}

Quat mulQuat(float t, Quat v1)
{
	return { t * v1.X, t * v1.Y, t * v1.Z, t * v1.W };
}

Quat plusQuat(Quat v0, Quat v1)
{
	return { v0.X + v1.X, v0.Y + v1.Y, v0.Z + v1.Z, v0.W + v1.W };
}
//
//Quat slerp(Quat qa, Quat qb, float t) {
//
//	Quat qm = Quat();
//	// Calculate angle between them.
//	double cosHalfTheta = qa.W * qb.W + qa.X * qb.X + qa.Y * qb.Y + qa.Z * qb.Z;
//	// if qa=qb or qa=-qb then theta = 0 and we can return qa
//	if (abs(cosHalfTheta) >= 1.0f) {
//		qm.W = qa.W; qm.X = qa.X; qm.Y = qa.Y; qm.Z = qa.Z;
//		return qm;
//	}
//	// Calculate temporary values.
//	double halfTheta = acos(cosHalfTheta);
//	double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
//	// if theta = 180 degrees then result is not fully defined
//	// we could rotate around any a.Xis normal to qa or qb
//	if (fabs(sinHalfTheta) < 0.001f) { // fabs is floating point absolute
//		qm.W = (qa.W * 0.5f + qb.W * 0.5f);
//		qm.X = (qa.X * 0.5f + qb.X * 0.5f);
//		qm.Y = (qa.Y * 0.5f + qb.Y * 0.5f);
//		qm.Z = (qa.Z * 0.5f + qb.Z * 0.5f);
//		return qm;
//	}
//	double ratioA = sin((1.f - t) * halfTheta) / sinHalfTheta;
//	double ratioB = sin(t * halfTheta) / sinHalfTheta;
//	//calculate Quaternion.
//	qm.W = (qa.W * ratioA + qb.W * ratioB);
//	qm.X = (qa.X * ratioA + qb.X * ratioB);
//	qm.Y = (qa.Y * ratioA + qb.Y * ratioB);
//	qm.Z = (qa.Z * ratioA + qb.Z * ratioB);
//	return qm;
//
//	// Only unit quaternions are valid rotations.
//	// Normalize to avoid undefined behavior.
//	//v0 = normalize(v0);
//	//v1 = normalize(v1);
//
//	//// Compute the cosine of the angle between the two vectors.
//	//double dot = v0.X * v1.X + v0.Y * v1.Y + v0.Z * v1.Z + v0.W * v1.W ;
//
//	//// If the dot product is negative, slerp won't take
//	//// the shorter path. Note that v1 and -v1 are equivalent when
//	//// the negation is applied to all four components. Fix by 
//	//// reversing one quaternion.
//	//if (dot < 0.0f) {
//	//	v1 = v1.conjugate();
//	//	v1 = mulQuat(1 / v1.X*v1.X + v1.Y * v1.Y + v1.Z * v1.Z + v1.W * v1.W, v1);
//	//	dot = -dot;
//	//}
//
//	//const double DOT_THRESHOLD = 0.9995;
//	//if (dot > DOT_THRESHOLD) {
//	//	// If the inputs are too close for comfort, linearly interpolate
//	//	// and normalize the result.
//
//	//	Quat result = plusQuat(v0, mulQuat(t, minusQuat(v1, v0)));
//	//	result = normalize(result);
//	//	return result;
//	//}
//
//	//// Since dot is in range [0, DOT_THRESHOLD], acos is safe
//	//double theta_0 = acos(dot);        // theta_0 = angle between input vectors
//	//double theta = theta_0 * t;          // theta = angle between v0 and result
//	//double sin_theta = sin(theta);     // compute this value only once
//	//double sin_theta_0 = sin(theta_0); // compute this value only once
//
//	//double s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
//	//double s1 = sin_theta / sin_theta_0;
//
//	//return plusQuat(mulQuat(s0, v0), mulQuat(s1 , v1));
//}

Quat slerp(Quat q1, Quat q2, float t)
{
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
	Quat q2New;
	float theta, mult1, mult2;

	w1 = q1.W; x1 = q1.X; y1 = q1.Y; z1 = q1.Z;
	w2 = q2.W; x2 = q2.X; y2 = q2.Y; z2 = q2.Z;

	// Reverse the sign of q2 if q1.q2 < 0.
	if (w1*w2 + x1 * x2 + y1 * y2 + z1 * z2 < 0)
	{
		w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
	}

	theta = acos(w1*w2 + x1 * x2 + y1 * y2 + z1 * z2);

	if (theta > 0.000001)
	{
		mult1 = sin((1 - t)*theta) / sin(theta);
		mult2 = sin(t*theta) / sin(theta);
	}

	// To avoid division by 0 and by very small numbers the approximation of sin(angle)
	// by angle is used when theta is small (0.000001 is chosen arbitrarily).
	else
	{
		mult1 = 1.f - t;
		mult2 = t;
	}

	w3 = mult1 * w1 + mult2 * w2;
	x3 = mult1 * x1 + mult2 * x2;
	y3 = mult1 * y1 + mult2 * y2;
	z3 = mult1 * z1 + mult2 * z2;

	return Quat(w3, x3, y3, z3);
}


void CPPBM::onLoad()
{
	replayFile = std::make_shared<CPPRP::ReplayFile>("K:/Alpaca1000/0E42C57743B81717DBD855B1085F2D79.replay");
	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
		bind(&CPPBM::OnTick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick",
		bind(&CPPBM::GVCTick, this, std::placeholders::_1));

	replayFile->Load();
	replayFile->DeserializeHeader();
	//for (auto it : replayFile->GetProperty<std::vector<std::unordered_map<std::string, std::shared_ptr<CPPRP::Property>>>>("PlayerStats"))
	{
		//for (auto it2 : it)
		{
		//	printf("%s\n", it2.first.c_str());
		}
	}

	replayFile->tickables.push_back([&](const CPPRP::Frame f, const std::unordered_map<int, CPPRP::ActorStateData>& actorStats)
	{
		for (auto& actor : actorStats)
		{
			std::shared_ptr<CPPRP::TAGame::Car_TA> car = std::dynamic_pointer_cast<CPPRP::TAGame::Car_TA>(actor.second.actorObject);
			if (car)
			{

				cvarManager->log("replicatedsteer " + std::to_string((int)car->ReplicatedSteer));
				ReplicationData rd;
				rd.rbState = car->ReplicatedRBState;
				rd.steer = car->ReplicatedSteer;
				rd.throttle = car->ReplicatedThrottle;
				rd.handbrake = car->bReplicatedHandbrake;
				rd.delta = replayFile->frames[f.frameNumber].time - replayFile->frames[0].time;
				locations[f.frameNumber][actor.first] = rd;
				
				cvarManager->log("replicatedsteer " + std::to_string(car->ReplicatedRBState.position.x) + "|" + std::to_string(car->ReplicatedRBState.position.y) + "|" + std::to_string(car->ReplicatedRBState.position.z) + "|" + std::to_string(rd.steer) );
			}
		}
	});
	replayFile->Parse();

	const uint32_t numFrames = static_cast<uint32_t>(replayFile->GetProperty<int32_t>("NumFrames"));

	for (auto& act : locations[0])
	{

		for (uint32_t frame = 0; frame < numFrames; )
		{
			//cvarManager->log("test");
			auto foundHere = locations[frame].find(act.first);
			
			if (foundHere != locations[frame].end())
			{
				int framesAdd = 1;
				while (true)
				{
					auto foundNext = locations[frame + framesAdd].find(act.first);
					if (foundNext == locations[frame + framesAdd].end())
					{
						--foundNext;
						--framesAdd;
						break;
					}

					if (abs(foundNext->second.rbState.position.x - foundHere->second.rbState.position.x) > .1
						|| abs(foundNext->second.rbState.position.y - foundHere->second.rbState.position.y) > .1
						|| abs(foundNext->second.rbState.position.z - foundHere->second.rbState.position.z) > .1)
						break;

					framesAdd++;
				}
				auto start = locations[frame][act.first];
				auto end = locations[frame + framesAdd][act.first];
				float diff = end.delta - start.delta;
				for (int i = frame + 1; i < frame + framesAdd; i++)
				{
					float elaps = locations[i][act.first].delta - start.delta;
					float ff =  (elaps / diff);

					Quat oldQuat(start.rbState.rotation.w, start.rbState.rotation.x, start.rbState.rotation.y, start.rbState.rotation.z);
					Quat nextQuat(end.rbState.rotation.w, end.rbState.rotation.x, end.rbState.rotation.y, end.rbState.rotation.z);
					Quat slerped = slerp(oldQuat, nextQuat, 1.f - ff);
					
					locations[i][act.first].rbState.position = {
						start.rbState.position.x + (end.rbState.position.x - start.rbState.position.x) * ff,
						start.rbState.position.y + (end.rbState.position.y - start.rbState.position.y) * ff,
						start.rbState.position.z + (end.rbState.position.z - start.rbState.position.z) * ff,
					};
					locations[i][act.first].rbState.linear_velocity = {
						start.rbState.linear_velocity.x * (1.f - ff) + end.rbState.linear_velocity.x * (ff),
						start.rbState.linear_velocity.y * (1.f - ff) + end.rbState.linear_velocity.y * (ff),
						start.rbState.linear_velocity.z * (1.f - ff) + end.rbState.linear_velocity.z * (ff)
					};
					locations[i][act.first].rbState.angular_velocity = {
						start.rbState.angular_velocity.x * (1.f - ff) + end.rbState.angular_velocity.x * (ff),
						start.rbState.angular_velocity.y * (1.f - ff) + end.rbState.angular_velocity.y * (ff),
						start.rbState.angular_velocity.z * (1.f - ff) + end.rbState.angular_velocity.z * (ff)
					};
					//locations[i][act.first].rbState.rotation = { slerped.X, slerped.Y, slerped.Z, slerped.W };
					
				}
				cvarManager->log("diff: " + std::to_string(framesAdd));
				frame += framesAdd == 0 ? 1 : framesAdd;
			}
			else
			{
				frame++;
			}

		}
	}


	for (auto& act : locations[0])
	{

		for (uint32_t frame = 0; frame < numFrames; )
		{
			//cvarManager->log("test");
			auto foundHere = locations[frame].find(act.first);

			if (foundHere != locations[frame].end())
			{
				int framesAdd = 1;
				while (true)
				{
					auto foundNext = locations[frame + framesAdd].find(act.first);
					if (foundNext == locations[frame + framesAdd].end())
					{
						--foundNext;
						--framesAdd;
						break;
					}

					if (abs(foundNext->second.rbState.rotation.x - foundHere->second.rbState.rotation.x) > .001
						|| abs(foundNext->second.rbState.rotation.y - foundHere->second.rbState.rotation.y) > .001
						|| abs(foundNext->second.rbState.rotation.z - foundHere->second.rbState.rotation.z) > .001
						|| abs(foundNext->second.rbState.rotation.w - foundHere->second.rbState.rotation.w) > .001)
						break;

					framesAdd++;
				}
				auto start = locations[frame][act.first];
				auto end = locations[frame + framesAdd][act.first];
				float diff = end.delta - start.delta;
				for (int i = frame + 1; i < frame + framesAdd; i++)
				{
					float elaps = locations[i][act.first].delta - start.delta;
					float ff = (elaps / diff);

					Quat oldQuat(start.rbState.rotation.w, start.rbState.rotation.x, start.rbState.rotation.y, start.rbState.rotation.z);
					Quat nextQuat(end.rbState.rotation.w, end.rbState.rotation.x, end.rbState.rotation.y, end.rbState.rotation.z);
					Quat slerped = slerp(oldQuat, nextQuat, ff);

					locations[i][act.first].rbState.position = {
						start.rbState.position.x + (end.rbState.position.x - start.rbState.position.x) * ff,
						start.rbState.position.y + (end.rbState.position.y - start.rbState.position.y) * ff,
						start.rbState.position.z + (end.rbState.position.z - start.rbState.position.z) * ff,
					};
					locations[i][act.first].rbState.linear_velocity = {
						start.rbState.linear_velocity.x * (1.f - ff) + end.rbState.linear_velocity.x * (ff),
						start.rbState.linear_velocity.y * (1.f - ff) + end.rbState.linear_velocity.y * (ff),
						start.rbState.linear_velocity.z * (1.f - ff) + end.rbState.linear_velocity.z * (ff)
					};
					locations[i][act.first].rbState.angular_velocity = {
						start.rbState.angular_velocity.x * (1.f - ff) + end.rbState.angular_velocity.x * (ff),
						start.rbState.angular_velocity.y * (1.f - ff) + end.rbState.angular_velocity.y * (ff),
						start.rbState.angular_velocity.z * (1.f - ff) + end.rbState.angular_velocity.z * (ff)
					};
					//locations[i][act.first].rbState.rotation = { slerped.X, slerped.Y, slerped.Z, slerped.W };

				}
				cvarManager->log("diff: " + std::to_string(framesAdd));
				frame += framesAdd == 0 ? 1 : framesAdd;
			}
			else
			{
				frame++;
			}

		}
	}


	currentFrame = locations.begin()->first;
	currentFrameIngame = 0;
}

void CPPBM::onUnload()
{
}

void CPPBM::GVCTick(std::string name)
{
	auto found = locations.find(currentFrame);
	auto nextFound = locations.find(currentFrame + 1);
	if (found == locations.end() || nextFound == locations.end())
		return;
	//for (auto loc2 : found->second)
	//{
	//	cvarManager->log(" - " + std::to_string(loc2.second.steer));
	//}
	auto actorToGetLoc = ++found->second.begin();
	auto nextActorToGetLoc = ++nextFound->second.begin();
	if (actorToGetLoc == found->second.end() || nextActorToGetLoc == nextFound->second.end())
		return;
	cvarManager->log(std::to_string(actorToGetLoc->second.steer));
	CPPRP::ReplicatedRBState rbState = actorToGetLoc->second.rbState;
	CPPRP::ReplicatedRBState nextRbState = nextActorToGetLoc->second.rbState;
	if (gameWrapper->GetGameEventAsServer().GetCars().Count() == 0)return;
	auto cw = gameWrapper->GetGameEventAsServer().GetCars().Get(0);
	auto carRbState = cw.GetCurrentRBState();

	float delta = nextActorToGetLoc->second.delta - actorToGetLoc->second.delta;
	
	std::chrono::duration<float, std::milli> applied2 = std::chrono::system_clock::now() - timeApplied;
	float applied = applied2.count() / 1000.f;
	
	float diff = applied - actorToGetLoc->second.delta;
	
	float ff = (diff / delta);
	//float ff = applied / .33f;
	//cvarManager->log("Perc:" + std::to_string(ff) + "|" + std::to_string(currentFrame));
	
	Quat oldQuat(rbState.rotation.w, rbState.rotation.x, rbState.rotation.y, rbState.rotation.z);
	Quat nextQuat(nextRbState.rotation.w, nextRbState.rotation.x, nextRbState.rotation.y, nextRbState.rotation.z);
	carRbState.Quaternion = slerp(oldQuat, nextQuat, ff);

	
	//{ rbState.rotation.w * ff + nextRbState.rotation.w * (1.f - ff),
	//	rbState.rotation.x * ff + nextRbState.rotation.x * (1.f - ff),
	//	rbState.rotation.y * ff + nextRbState.rotation.y * (1.f - ff) ,
	//	rbState.rotation.z * ff + nextRbState.rotation.z * (1.f - ff) };

	carRbState.Location = {
		rbState.position.x +  (nextRbState.position.x  - rbState.position.x ) * ff,
		rbState.position.y + (nextRbState.position.y - rbState.position.y) * ff,
		rbState.position.z + (nextRbState.position.z - rbState.position.z) * ff
	};
	cvarManager->log("Interped " + std::to_string(ff) + " | " + std::to_string(carRbState.Quaternion.W) + " > " + std::to_string(carRbState.Quaternion.X) + "|" + std::to_string(carRbState.Quaternion.Y) + "|" + std::to_string(carRbState.Quaternion.Z) + "|" + std::to_string(carRbState.Location.X) + "|" + std::to_string(carRbState.Location.Y) + "|" + std::to_string(carRbState.Location.Z) /*+ std::to_string(framesAdd)*/);

	carRbState.LinearVelocity = {
		rbState.linear_velocity.x * ff + nextRbState.linear_velocity.x * (1.f - ff),
		rbState.linear_velocity.y * ff + nextRbState.linear_velocity.y * (1.f - ff),
		rbState.linear_velocity.z * ff + nextRbState.linear_velocity.z * (1.f - ff)
	};
	carRbState.AngularVelocity = {
		rbState.angular_velocity.x * ff + nextRbState.angular_velocity.x * (1.f - ff),
		rbState.angular_velocity.y * ff + nextRbState.angular_velocity.y * (1.f - ff),
		rbState.angular_velocity.z * ff + nextRbState.angular_velocity.z * (1.f - ff)
	};
	//cw.SetLocation(carRbState.Location);
	//cw.GetCollisionComponent().SetbDisableAllRigidBody(true);
	//cw.SetRBState(carRbState);
	if (diff + (1.f / 150.f) > delta) currentFrame++;
	//cw.SetLocation(carRbState.Location);
}

void CPPBM::OnTick(CarWrapper cw, void * params, std::string funcName)
{
	static bool b = false;
	if (!b)
	{
		b = true;
		for (auto loc : locations)
		{
			for (auto loc2 : loc.second)
			{
				cvarManager->log(std::to_string(loc.first) + " - " + std::to_string(loc2.second.steer));
			}
		}
	}
	auto found = locations.find(currentFrame);
	auto nextFound = locations.find(currentFrame + 1);
	if (found == locations.end() || nextFound == locations.end())
		return;
	for (auto loc2 : found->second)
	{
		//cvarManager->log(" - " + std::to_string(loc2.second.steer));
	}
	auto actorToGetLoc = (++found->second.begin());
	auto nextActorToGetLoc = (++nextFound->second.begin());
	if (actorToGetLoc == found->second.end() || nextActorToGetLoc == nextFound->second.end())
		return;
	//cvarManager->log(std::to_string(actorToGetLoc->second.steer));
	CPPRP::ReplicatedRBState rbState = actorToGetLoc->second.rbState;
	CPPRP::ReplicatedRBState nextRbState = nextActorToGetLoc->second.rbState;
	//cw.SetLocation({ cppRPvec.x, cppRPvec.y, cppRPvec.z});
	
	auto carRbState = cw.GetCurrentRBState();
	float ff = 1.f - (1.f / (float)((currentFrameIngame % 4) + 1));
	//cvarManager->log(std::to_string(ff));
	/*carRbState.Quaternion = { rbState.rotation.w * ff + rbState.rotation.w * (1.f - ff), 
		rbState.rotation.x * ff + nextRbState.rotation.x * (1.f - ff),
		rbState.rotation.y * ff + nextRbState.rotation.y * (1.f - ff) ,
		rbState.rotation.z * ff + nextRbState.rotation.z * (1.f - ff) };*/

	//carRbState.Quaternion = slerp(rbState.rotation, nextRbState.rotation, ff);
	carRbState.Location = { 
		rbState.position.x * ff + nextRbState.position.x * (1.f -ff),
		rbState.position.y * ff + nextRbState.position.y * (1.f - ff),
		rbState.position.z * ff + nextRbState.position.z * (1.f - ff)
	};
	carRbState.LinearVelocity = { 
		rbState.linear_velocity.x * ff + nextRbState.linear_velocity.x * (1.f - ff),
		rbState.linear_velocity.y * ff + nextRbState.linear_velocity.y * (1.f - ff),
		rbState.linear_velocity.z * ff + nextRbState.linear_velocity.z * (1.f - ff)
	};
	carRbState.AngularVelocity = { 
		rbState.angular_velocity.x * ff + nextRbState.angular_velocity.x * (1.f - ff),
		rbState.angular_velocity.y * ff + nextRbState.angular_velocity.y * (1.f - ff),
		rbState.angular_velocity.z * ff + nextRbState.angular_velocity.z * (1.f - ff)
	};
	cw.SetRBState(carRbState);

	/*carRbState.Quaternion = { rbState.rotation.w, rbState.rotation.x, rbState.rotation.y, rbState.rotation.z };
	carRbState.Location = { rbState.position.x, rbState.position.y, rbState.position.z };
	carRbState.LinearVelocity = { rbState.linear_velocity.x, rbState.linear_velocity.y, rbState.linear_velocity.z };
	carRbState.AngularVelocity = { rbState.angular_velocity.x, rbState.angular_velocity.y, rbState.angular_velocity.z };
	*/
	
	//cw.SetLocation(carRbState.Location);
	//cw.SetVelocity(carRbState.LinearVelocity);
	//cw.SetAngularVelocity(carRbState.AngularVelocity, 0);
	
	/*cvarManager->log("Old throttle: " + std::to_string((int)(actorToGetLoc->second.throttle)));
	cvarManager->log("Throttle " + std::to_string(((float)(actorToGetLoc->second.throttle - 128) / 128.f)));
	

	cvarManager->log("Old steer: " + std::to_string((int)(actorToGetLoc->second.steer)));
	cvarManager->log("Steer " + std::to_string(((float)(actorToGetLoc->second.steer - 128) / 128.f)));*/
	ControllerInput* input = (ControllerInput*)params;
	input->Steer = ((float)(actorToGetLoc->second.steer - 128) / 128.f) * ff + ((float)(nextActorToGetLoc->second.steer - 128) / 128.f) * (1.f-ff);

	input->Throttle = ((float)(actorToGetLoc->second.throttle - 128) / 128.f) * ff + ((float)(nextActorToGetLoc->second.throttle - 128) / 128.f) * (1.f - ff);
	input->Handbrake = (actorToGetLoc->second.handbrake);
	//cw.SetInput(inp);
	//
	//cw.SetbUpdateSimulatedPosition(true);
	//cw.SetbReplayActor(true);
	//cw.SetbReplicateRigidBodyLocation(true);
	//cw.SetReplicatedRBState(carRbState);
	//cw.SetRBState(carRbState);
	//cw.SetOldRBState(carRbState);
	//cw.SetClientCorrectionRBState(carRbState);
	
	if (currentFrameIngame == 0)
	{
		timeApplied = std::chrono::system_clock::now();
		//timeApplied = gameWrapper->GetGameEventAsServer().GetSecondsElapsed();
	}
	if (currentFrameIngame% 4 == 0) 
	{
		//cw.SetPhysicsState(carRbState);
		/*cw.SetLocation(carRbState.Location);
		cw.SetVelocity(carRbState.LinearVelocity);
		cw.SetAngularVelocity(carRbState.AngularVelocity, 0);*/
		//cw.SetPhysicsState(carRbState);
		//currentFrame++;
		
	}
	
	currentFrameIngame++;
	//cvarManager->log("Tick");
}
