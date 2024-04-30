#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "run.hh"
#include "stepping.hh"
#include "event.hh"

class myActionInitialization : public G4VUserActionInitialization {
    public:
        myActionInitialization();
        ~myActionInitialization();

        virtual void Build() const;
        virtual void BuildForMaster() const;
};

#endif