#include<iostream>
#include<vector>
using namespace std;

enum RideStatus {IDLE, CREATED, WITHDRAWN, COMPLETED};

class Ride{
    private:
        int id;
        int origin, dest;
        int seats;
        RideStatus rideStatus;
    public:
        static const int AMT_PER_KM= 20;
        Ride();
        int calculateFare(bool);
        void setDest(int dest);
        int getId() const;
        void setId(int id);
        void setOrigin(int origin);
        RideStatus getRideStatus() const;
        void setRideStatus(RideStatus rideStatus);
        void setSeats(int seats);

};
class Person{
    public:
        string name;
};
class Rider : private Person{

    private:
        vector<Ride> completedRides;
        Ride currentRide;
    public:
        Rider(string);

        void createRide(int,int,int,int);
        void updateRide(int,int,int,int);
        void withdrawRide(int);
        int closeRide();


};
class Driver : private Person{

    public:
        Driver(string);

};

int Ride::calculateFare(bool isPriorityRider){

    int dist = dest-origin;
    if(seats<2){
            return dist*AMT_PER_KM*(isPriorityRider?0.75:1);
    }
    else{

        return dist*seats*AMT_PER_KM*(isPriorityRider?0.50:0.75);
    }
}

Ride::Ride(){

    id=origin=dest=seats=0;
    rideStatus = RideStatus::IDLE;
}

Driver::Driver(string name){

    this->name=name;
}

void Ride::setDest(int dest){
    this->dest=dest;
}

int Ride::getId() const{
    return id;
}

void Ride::setId(int id){
    this->id = id;
}

void Ride::setOrigin(int origin){
    this->origin = origin;
}

RideStatus Ride:: getRideStatus() const{
    return rideStatus;
}

void Ride:: setRideStatus(RideStatus rideStatus){
    this->rideStatus = rideStatus;
}

void Ride:: setSeats(int seats){
    this->seats=seats;
}

Rider::Rider(string name){
    this->name=name;
}

void Rider::createRide(int id, int origin, int dest, int seats){

    if(dest<=origin){
        cout<<"can't  create a ride\n";
    }
    currentRide.setId(id);
    currentRide.setOrigin(origin);
    currentRide.setDest(dest);
    currentRide.setSeats(seats);
    currentRide.setRideStatus(RideStatus::CREATED);

}

void Rider::updateRide(int id, int origin, int dest, int seats){

    if(currentRide.getRideStatus()== RideStatus::WITHDRAWN){

        cout<<"Can't Update ride, Ride was withdrawn.\n";
        return;
    }

    if(currentRide.getRideStatus()== RideStatus::COMPLETED){

        cout<<"Can't Update ride, Ride was already Completed.\n";
        return;
    }
    createRide(id,origin,dest,seats);

}

void Rider::withdrawRide(int id){

    if(currentRide.getId()!=id){
        cout<<"Wrong id as a input. Please give a valid input\n";
        return;
    }

    if(currentRide.getRideStatus()== RideStatus::CREATED){

        cout<<"Can't Withdraw ride, Ride was not created till.\n";
        return;
    }

    currentRide.setRideStatus(RideStatus::WITHDRAWN);
}

int Rider::closeRide(){

    if(currentRide.getRideStatus()!= RideStatus::CREATED){

        cout<<"Can't close ride, Ride was not created till.\n";
        return 0;
    }
    currentRide.setRideStatus(RideStatus::COMPLETED);
    completedRides.push_back(currentRide);
    return currentRide.calculateFare(completedRides.size()>=10);


}

int main(){
    cout<<"Hello world\n";

    Rider rider("A");
    Driver driver("d");
    rider.createRide(1,50,60,1);
    cout<<rider.closeRide()<<"\n";
    rider.updateRide(1,50,60,2);
    cout<<rider.closeRide()<<"\n";

    cout<<"***************************************************************************\n";

    rider.createRide(1,50,60,1);
    rider.withdrawRide(1);
    rider.updateRide(1,50,60,2);
    cout<<rider.closeRide()<<"\n";

    cout<<"***************************************************************************\n";

     rider.createRide(1,50,60,1);
    rider.updateRide(1,50,60,2);
    cout<<rider.closeRide()<<"\n";

    cout<<"***************************************************************************\n";
    return 0;
}