//! \file *****************************************************************
//! ***********************************************************************
//!  Module : MC_Kernel
//! ***********************************************************************
//! *
//! * The following comments will be automatically
//! * inserted by the source control tool:
//! *
//! * $HeadURL: http://svnserver/svn/modules/MC_Kernel/tags/trunk/src/MotionHomeSM_DoNotChange.hpp $
//! * $Author:  ASPERG\dwedel (authomatically generated)$ 
//! * $Date:    27.05.2019 17:59:16 $
//! * $Revision: SVN-REVISION $
//! *
//! ***********************************************************************
//! * \brief
//! *	<A short description of the file>
//! ***********************************************************************
//! * Copyright (c) 2019 by Jetter AG, Ludwigsburg, Germany
//! ***********************************************************************

#ifndef _MOTIONHOMESM_HPP_
#define _MOTIONHOMESM_HPP_

/* includes */
#include "src/OS.h"
#include "src/OS_Queue.h"

/* forward declarations */
class BlaClass;
struct qqq;

namespace MotionHome
{

    /****************** EventID begin ****************************************/
    /*
     * This are events, which will be handled by FSM
     */
    enum EventID
    {
        CYCLIC_EVENT,
        DRIVE_READ_DATA,
        PARAM_UPDATE_HOMING,
        AXIS_USER_REF_START,
        AXIS_USER_REF_SET,
        AXIS_USER_REF_HALT,
        AXIS_STOPPED,
        CAM_DETECTED,
        K0_DETECTED,
        NUM_OF_EVENTID
    };
    /****************** EventID end ******************************************/


    /****************** KIND_REFERENCE_MARK begin ****************************/
    /*
     * options for reference mark
     */
    enum KIND_REFERENCE_MARK
    {
        UNDEFINED_REF_MARK,
        ZERO_MARK,
        SLOPE
    };
    /****************** KIND_REFERENCE_MARK end ******************************/


    /****************** NUMBER_PHASE begin ***********************************/
    /*
     * describes how many phases should the motion home take
     */
    enum NUMBER_PHASE
    {
        UNDEFINED_PHASE_NUMBER,
        ONE,
        TWO
    };
    /****************** NUMBER_PHASE end *************************************/


    /****************** SLOPE_POSITION_TO_SEARCH begin ***********************/
    /*
     * What is the current slope position to search
     */
    enum SLOPE_POSITION_TO_SEARCH
    {
        UNDEFINED_SLOPE_POSITION,
        NO_SLOPE,
        REF_SWITCH_LEFT,
        REF_SWITCH_RIGHT,
        NEGATIVE_SWITCH,
        POSITIVE_SWITCH
    };
    /****************** SLOPE_POSITION_TO_SEARCH end *************************/


    /****************** KIND_OF_SLOPE begin **********************************/
    /*
     * What is the current slope to search
     */
    enum KIND_OF_SLOPE
    {
        UNDEFINED_KIND_OF_SLOPE,
        HIGH_LEFT,
        LOW_LEFT,
        HIGH_RIGHT,
        LOW_RIGHT,
        HIGH_NEGATIVE,
        HIGH_POSITIVE,
        LOW_NEGATIVE,
        LOW_POSITIVE
    };
    /****************** KIND_OF_SLOPE end ************************************/


    /****************** SWITCH_STATE begin ***********************************/
    /*
     * describes the axis position in relation to switch
     */
    enum SWITCH_STATE
    {
        UNDEFINED_SWITCH_STATE,
        OFF_SWITCH,
        ON_MIDDLE_SWITCH,
        ON_NEGATIVE_SWITCH,
        ON_POSITIVE_SWITCH
    };
    /****************** SWITCH_STATE end *************************************/


    /****************** ERROR_TYPE begin *************************************/
    /*
     * which error could happen
     */
    enum ERROR_TYPE
    {
        UNDEFINED_ERROR_TYPE
    };
    /****************** ERROR_TYPE end ***************************************/

    class MotionHomeSM
    {
    private:

        /****************** CGeneralState begin ******************************/
        class CGeneralState
        {
        public:
            CGeneralState(MotionHomeSM* fsm, CGeneralState* f_superState, bool withHistory);
            virtual ~CGeneralState();

            virtual bool dispatch(const EventID eMessage) = 0;
            virtual const char* toString() const = 0;

            virtual void enter() {}
            virtual void exit() {}

            CGeneralState* getSuperState() const;
            CGeneralState* getCurrentSubState() const;
            void setCurrentSubState(CGeneralState& state);
            void updateSubState();

            CGeneralState& findCommonFatherWith(CGeneralState& other) const;

        protected:
            MotionHomeSM* m_fsm;
            CGeneralState* m_superState;
            CGeneralState* m_currentSubState;
            CGeneralState* m_startSubState;

        private:
            bool m_withHistory_b;
            int getDepth() const;
        };
        /****************** CGeneralState end ********************************/


        /****************** CRoot begin **************************************/
        class CRoot : public CGeneralState
        {
        public:
            CRoot(MotionHomeSM* fsm);
            virtual ~CRoot();

            virtual bool dispatch(const EventID eMessage);
            virtual const char* toString() const;
        };
        friend CRoot;

        CRoot mRoot;
        /****************** CRoot end ****************************************/


        /****************** CWorking begin ***********************************/
        /*
         * super state for working part
         */
        class CWorking : public CGeneralState
        {
        public:
            CWorking(MotionHomeSM* fsm);
            virtual ~CWorking();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);

            bool isErrorOccured() const;

        };
        friend CWorking;

        CWorking mWorking;
        /****************** CWorking end *************************************/


        /****************** CStart_finish begin ******************************/
        /*
         * the SM starts and ends here
         */
        class CStart_finish : public CGeneralState
        {
        public:
            CStart_finish(MotionHomeSM* fsm);
            virtual ~CStart_finish();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool isExternalAxisAndMotionHomeActive() const;

            bool isMotionHomeActiveAndAxisHold() const;
            void startMotionHome();

            bool isMotionHomeActiveButAxisInMotion() const;
            void processHoldOn();

            void processStartMotionHome();

            void processSetMotionHome();

            void updateParameters();

            bool isSetReferenceActive() const;


        private:
            /*
             * is motion home started
             */
            bool isStarted;
            /*
             * is motion home (already) set
             */
            bool isSet;
            /*
             * is this an external axis?
             */
            bool isExternal;
        };
        friend CStart_finish;

        CStart_finish mStart_finish;
        /****************** CStart_finish end ********************************/


        /****************** CMotion_home_ongoing begin ***********************/
        /*
         * handle all situations during running motion home
         */
        class CMotion_home_ongoing : public CGeneralState
        {
        public:
            CMotion_home_ongoing(MotionHomeSM* fsm);
            virtual ~CMotion_home_ongoing();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            void processHalt();

            void processAxisStopped();

        };
        friend CMotion_home_ongoing;

        CMotion_home_ongoing mMotion_home_ongoing;
        /****************** CMotion_home_ongoing end *************************/


        /****************** CInit begin **************************************/
        /*
         * prepare everything to motion home procedure
         */
        class CInit : public CGeneralState
        {
        public:
            CInit(MotionHomeSM* fsm);
            virtual ~CInit();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool isZmOnly() const;
            void startVzm();

            bool isOnePhase() const;

            bool isOffSwitchAndTwoPhase() const;
            void setCorrectSlope();

            bool isOnSwitchAndTwoPhase() const;

        };
        friend CInit;

        CInit mInit;
        /****************** CInit end ****************************************/


        /****************** CStart_outside_ref_switch begin ******************/
        /*
         * if  start mh outside of switch (usual case)
         */
        class CStart_outside_ref_switch : public CGeneralState
        {
        public:
            CStart_outside_ref_switch(MotionHomeSM* fsm);
            virtual ~CStart_outside_ref_switch();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            void processCamDetected();

            bool isCorrectSwitch() const;
            void setCorrectNextSlope();

            bool isMiddleSwitchButFoundHighSlopeForCorrectEndSwitch() const;
            void changeDirection();

        };
        friend CStart_outside_ref_switch;

        CStart_outside_ref_switch mStart_outside_ref_switch;
        /****************** CStart_outside_ref_switch end ********************/


        /****************** CStart_on_switch begin ***************************/
        /*
         * hgsi
         */
        class CStart_on_switch : public CGeneralState
        {
        public:
            CStart_on_switch(MotionHomeSM* fsm);
            virtual ~CStart_on_switch();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool isWrongSwitch() const;

            bool isCorrectSwitch() const;
            void setCorrectNextSlope();

        };
        friend CStart_on_switch;

        CStart_on_switch mStart_on_switch;
        /****************** CStart_on_switch end *****************************/


        /****************** CEnd_switch_low_high begin ***********************/
        /*
         * dhdhd
         */
        class CEnd_switch_low_high : public CGeneralState
        {
        public:
            CEnd_switch_low_high(MotionHomeSM* fsm);
            virtual ~CEnd_switch_low_high();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool hasFoundAnotherEndSwitch() const;
            void setError();

            void processCamDetected();

            bool hasFoundCorrectSlope() const;

        };
        friend CEnd_switch_low_high;

        CEnd_switch_low_high mEnd_switch_low_high;
        /****************** CEnd_switch_low_high end *************************/


        /****************** CCorrect_switch_fast_low_high begin **************/
        /*
         * fjboidbh
         */
        class CCorrect_switch_fast_low_high : public CGeneralState
        {
        public:
            CCorrect_switch_fast_low_high(MotionHomeSM* fsm);
            virtual ~CCorrect_switch_fast_low_high();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool notFoundCorrectSlope() const;
            void setError();

            void processCamDetected();

            bool foundCorrectSlope() const;

        };
        friend CCorrect_switch_fast_low_high;

        CCorrect_switch_fast_low_high mCorrect_switch_fast_low_high;
        /****************** CCorrect_switch_fast_low_high end ****************/


        /****************** CStart_slow_motion begin *************************/
        /*
         * slow motion to find reference
         */
        class CStart_slow_motion : public CGeneralState
        {
        public:
            CStart_slow_motion(MotionHomeSM* fsm);
            virtual ~CStart_slow_motion();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool notFoundCorrectSlope() const;
            void setError();

            void processCamDetected();

            bool foundCorrectSlope() const;

        };
        friend CStart_slow_motion;

        CStart_slow_motion mStart_slow_motion;
        /****************** CStart_slow_motion end ***************************/


        /****************** CCorrect_switch_high_low begin *******************/
        /*
         * fhdth
         */
        class CCorrect_switch_high_low : public CGeneralState
        {
        public:
            CCorrect_switch_high_low(MotionHomeSM* fsm);
            virtual ~CCorrect_switch_high_low();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);

            bool isMiddleSwitch() const;
            void changeDirection();

            bool isZmAndEndSwitch() const;

            bool isNotZmAndEndSwitch() const;

        };
        friend CCorrect_switch_high_low;

        CCorrect_switch_high_low mCorrect_switch_high_low;
        /****************** CCorrect_switch_high_low end *********************/


        /****************** CCorrect_switch_slow_low_high begin **************/
        /*
         * sgrdsghadr
         */
        class CCorrect_switch_slow_low_high : public CGeneralState
        {
        public:
            CCorrect_switch_slow_low_high(MotionHomeSM* fsm);
            virtual ~CCorrect_switch_slow_low_high();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);

            bool isZmActive() const;

            bool isZmNotActive() const;

        };
        friend CCorrect_switch_slow_low_high;

        CCorrect_switch_slow_low_high mCorrect_switch_slow_low_high;
        /****************** CCorrect_switch_slow_low_high end ****************/


        /****************** CSearch_for_zm begin *****************************/
        /*
         * search for Zm
         */
        class CSearch_for_zm : public CGeneralState
        {
        public:
            CSearch_for_zm(MotionHomeSM* fsm);
            virtual ~CSearch_for_zm();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            void setError();

            void processZmSignal();

            bool isK0Detected() const;

        };
        friend CSearch_for_zm;

        CSearch_for_zm mSearch_for_zm;
        /****************** CSearch_for_zm end *******************************/


        /****************** CSet_reference_point begin ***********************/
        /*
         * set reference
         */
        class CSet_reference_point : public CGeneralState
        {
        public:
            CSet_reference_point(MotionHomeSM* fsm);
            virtual ~CSet_reference_point();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool isNotComplete() const;
            void setError();

            bool isComplete() const;
            void setReference();

        };
        friend CSet_reference_point;

        CSet_reference_point mSet_reference_point;
        /****************** CSet_reference_point end *************************/


        /****************** CExternal begin **********************************/
        /*
         * Satte for handling motion home of external axis
         */
        class CExternal : public CGeneralState
        {
        public:
            CExternal(MotionHomeSM* fsm);
            virtual ~CExternal();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            void readDriveData();

            bool isComplete() const;
            void reportEndOfMotionHomeExternal();

        };
        friend CExternal;

        CExternal mExternal;
        /****************** CExternal end ************************************/


        /****************** CSend_reference_data_to_drive begin **************/
        /*
         * communication with real hardware drive
         */
        class CSend_reference_data_to_drive : public CGeneralState
        {
        public:
            CSend_reference_data_to_drive(MotionHomeSM* fsm);
            virtual ~CSend_reference_data_to_drive();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

            bool hasDriveAnswered() const;
            void reportEndOfMotionHome();

        };
        friend CSend_reference_data_to_drive;

        CSend_reference_data_to_drive mSend_reference_data_to_drive;
        /****************** CSend_reference_data_to_drive end ****************/


        /****************** CError begin *************************************/
        /*
         * Error handling
         */
        class CError : public CGeneralState
        {
        public:
            CError(MotionHomeSM* fsm);
            virtual ~CError();

            virtual const char* toString() const;
            virtual bool dispatch(const EventID eMessage);
            virtual void enter();

        };
        friend CError;

        CError mError;
        /****************** CError end ***************************************/


        /*********************************************************************/
        // member FSM
        /*********************************************************************/
    public:
        MotionHomeSM();
        virtual ~MotionHomeSM();

        void processEvent(const EventID eMessage);

    private:
        // forbid copy constructor and assignment operator by declaring them private
        MotionHomeSM(const MotionHomeSM& other);
        MotionHomeSM& operator=(const MotionHomeSM& other);

        void switchState(CGeneralState& from, CGeneralState& to);

        /*
         * Because the processEvent handles fully automaticall (the event will be routed to
         * the state which handles it) here is the opportunity to write some special code
         * (the function is empty be default) which makes some general things (state independent)
         * BEFORE "processEvent" will be called even depend on eventID
         */
        void preProcess(const EventID eMessage);

        /*
         * Because the processEvent handles fully automaticall (the event will be routed to
         * the state which handles it) here is the opportunity to write some special code
         * (the function is empty be default) which makes some general things (state independent)
         * AFTER "processEvent" was called even depend on eventID
         */
        void postProcess(const EventID eMessage);

    private:

        // member data
        /*
         * moves the axis in positive direction currently
         */
        bool isCurrentDirectionPositive;
        /*
         * moves the axis in positive direction at the begin
         */
        bool isStartDirectionPositive;
        /*
         * is there a reference switch (middle)
         */
        bool hasReferenceSwitch;
        /*
         * are there switches at the ends
         */
        bool hasEndSwitches;
        /*
         * current mark
         */
        KIND_REFERENCE_MARK kindOfReferenceMark;
        /*
         * number of reference phases
         */
        NUMBER_PHASE numberOfPhases;
        /*
         * max acceleration of axis
         */
        double acceleration;
        /*
         * speed during searching for switch
         */
        double speedDuringSearchForSwitch;
        /*
         * maximal distance searching for switch
         */
        double maxDistanceSearchingForSwitch;
        /*
         * speed during searching for reference
         */
        double speedDuringSearchForReference;
        /*
         * maximal distance searching for reference
         */
        double maxDistanceSearchingForReference;
        /*
         * distance to null point 
         */
        double nullPointDistance;
        /*
         * position to null point
         */
        double nullPointPosition;
        /*
         * yyy
         */
        double referenceMark;
        /*
         * is motion home completed
         */
        bool isComplete;
        /*
         * error during motion home occured
         */
        bool isError;
        /*
         * which error is occured
         */
        ERROR_TYPE errorType;
        /*
         * is motion home started
         */
        bool isMotionHomeStarted;
        /*
         * last kind of slope
         */
        KIND_OF_SLOPE lastKindOfSlope;
        /*
         * is motion home set
         */
        bool isMotionHomeSet;
        /*
         * is update of motion home data desired
         */
        bool isUpdateOfMotionHomeDataDesired;
        /*
         * is user halt
         */
        bool isUserHalt;
        /*
         * is axis stopped
         */
        bool isAxisStopped;
        /*
         * is K0 detected
         */
        bool isK0Detected;
        /*
         * is cam detected
         */
        bool isCamDetected;

        // member function(s)
    };    // class MotionHomeSM

}    // namespace MotionHome

#endif    // #define _MOTIONHOMESM_HPP_

