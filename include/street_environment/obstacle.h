#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "lms/math/vertex.h"
#include "street_environment/road.h"

#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/base_class.hpp"

namespace street_environment{
/**
 * @brief A dynamic entity can be the vehicle itself but also every other
 * moving obstacle.
 */
class Obstacle:public EnvironmentObject, public lms::Serializable{

    std::vector<lms::math::vertex2f> m_points;

    /**
     * @brief stores the position kalman!
     */
    lms::math::vertex2f m_position;
    bool valid;

    void myValidate(){
        calculatePosition();
    }

    void calculatePosition(){
        m_position.x = 0;
        m_position.y = 0;
        if(m_points.size() == 0){
            return;
        }
        for(const lms::math::vertex2f &v:m_points){
            m_position +=v;
        }
        m_position = m_position/m_points.size();
    }


public:
    static constexpr int TYPE = 1;
    virtual int getType() const override{
       return TYPE;
    }
    virtual ~Obstacle() {}

    virtual bool match(const Obstacle &obj) const;
    Obstacle();
    /**
     * @brief init used to init the kalman!
     */
    bool m_init;

    void invalid(){
        valid = false;
    }
    bool isvalid(){
        return valid;
    }
    void validate(){
        if(!valid){
            myValidate();
            valid = true;
        }
    }
    std::vector<lms::math::vertex2f> points() const{
        return m_points;
    }

    lms::math::vertex2f viewDirection() const{
        return lms::math::vertex2f(1,0);//TODO
    }

    void addPoint(const lms::math::vertex2f &v){
        m_points.push_back(v);
    }
    void clearPoints(){
        m_points.clear();
    }


    lms::math::vertex2f position() const;
    //TODO add BoundingBox methods
    float width() const{
        return 0.3; //TODO
    }
    void width(float w){
        (void)w;
        //TODO
    }

    void translate(float dx, float dy){
        invalid();
        lms::math::vertex2f delta(dx,dy);
        for(int i = 0; i < (int) m_points.size(); i++){
            m_points[i] = m_points[i]-delta;
        }
    }

    /**
     * @brief distanceTang
     * @return the length to the object tangential to the road
     */
    float distanceTang() const;
    /**
     * @brief distanceOrth
     * @return  the length to the object orthogonal to the road
     */
    float distanceOrth() const;
    /**
     * @brief velocityTang
     * @return the velocity tang to the road
     */
    float velocityTang() const;
    /**
     * @brief velocityOrth
     * @return the velocity orth to the road
     */
    float velocityOrth() const;

    template<class Archive>
    void serialize(Archive & archive) {
        archive (
            cereal::base_class<street_environment::EnvironmentObject>(this),
            m_position);
    }

    // cereal implementation
    //get default interface for datamanager
    CEREAL_SERIALIZATION()

};
typedef std::shared_ptr<Obstacle> ObstaclePtr;

} //street_environment

//CEREAL_REGISTER_TYPE(street_environment::Obstacle)

namespace cereal {
    template <class Archive>
    struct specialize<Archive, street_environment::Obstacle, cereal::specialization::member_serialize> {};
      // cereal no longer has any ambiguity when serializing street_environment::Obstacle
}  // namespace cereal

#endif /* OBSTACLE_H */
