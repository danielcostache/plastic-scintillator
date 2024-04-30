#include "action.hh"

myActionInitialization::myActionInitialization() {

}

myActionInitialization::~myActionInitialization() {

}

void myActionInitialization::BuildForMaster() const {
    myRunAction *runAction = new myRunAction();
    SetUserAction(runAction);
}

void myActionInitialization::Build() const {
    myPrimaryGenerator *generator = new myPrimaryGenerator();
    SetUserAction(generator);

    myRunAction *runAction = new myRunAction();
    SetUserAction(runAction);

    myEventAction *eventAction = new myEventAction(runAction);
    SetUserAction(eventAction);
    
    mySteppingAction *steppingAction = new mySteppingAction(eventAction);
    SetUserAction(steppingAction);
}