#ifndef STREET_ENVIRONMENT_TRAJECTORY_H
#define STREET_ENVIRONMENT_TRAJECTORY_H
#include "lms/math/polyline.h"

#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/cerealizable.h>

namespace street_environment {

    struct TrajectoryPoint:public lms::Serializable{
        lms::math::vertex2f position;
        lms::math::vertex2f directory;
        float velocity;
        float distanceToMiddleLane;
        /**
         * @brief isRight
         * @return true if the TrajectoryPoint is on the left side of the middle of the road
         */
        bool isRight() const{
            return distanceToMiddleLane > 0;
        }

        TrajectoryPoint():position(0,0),directory(1,0),velocity(0),distanceToMiddleLane(0){

        }
        TrajectoryPoint(lms::math::vertex2f pos,lms::math::vertex2f viewDir, float velocity_, float distanceToMiddleLane_):position(pos),directory(viewDir),velocity(velocity_),distanceToMiddleLane(distanceToMiddleLane_){

        }
        CEREAL_SERIALIZATION()

        template<class Archive>
        void serialize(Archive &ar) {
            ar(position, directory, velocity,distanceToMiddleLane);
        }
    };

    class Trajectory: public std::vector<TrajectoryPoint>, public lms::Inheritance, public lms::Serializable {
    public:

        template<class Archive>
        void serialize(Archive &archive) {
            archive(cereal::base_class<std::vector<TrajectoryPoint>>( this ));
        }

        CEREAL_SERIALIZATION()

        virtual bool isSubType(size_t hashcode) const override{
            return hashcode == typeid(std::vector<TrajectoryPoint>).hash_code();
        }
    };
}

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(street_environment::Trajectory, cereal::specialization::member_serialize )

#endif //STREET_ENVIRONMENT_TRAJECTORY_H

