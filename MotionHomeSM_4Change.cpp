//! \file *****************************************************************
//! ***********************************************************************
//!  Module : MC_Kernel
//! ***********************************************************************
//! *
//! * The following comments will be automatically
//! * inserted by the source control tool:
//! *
//! * $HeadURL: http://svnserver/svn/modules/MC_Kernel/tags/trunk/src/MotionHomeSM_4Change.cpp $
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

#include "MotionHomeSM_DoNotChange.hpp"

namespace MotionHome
{

    // ***********************************************************************
    //                            MotionHomeSM
    // ***********************************************************************


    // preProcess
    void MotionHomeSM::preProcess(const EventID eMessage)
    {
        // place your code here...
    }


    // postProcess
    void MotionHomeSM::postProcess(const EventID eMessage)
    {
        // place your code here...
    }


    // ***********************************************************************
    //                            CWorking
    // ***********************************************************************


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CWorking::isErrorOccured() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CStart_finish
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_finish::isExternalAxisAndMotionHomeActive() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_finish::isMotionHomeActiveAndAxisHold() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::startMotionHome()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_finish::isMotionHomeActiveButAxisInMotion() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::processHoldOn()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::processStartMotionHome()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::processSetMotionHome()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_finish::updateParameters()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_finish::isSetReferenceActive() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CMotion_home_ongoing
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CMotion_home_ongoing::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CMotion_home_ongoing::processHalt()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CMotion_home_ongoing::processAxisStopped()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CInit
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CInit::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CInit::isZmOnly() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CInit::startVzm()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CInit::isOnePhase() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CInit::isOffSwitchAndTwoPhase() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CInit::setCorrectSlope()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CInit::isOnSwitchAndTwoPhase() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CStart_outside_ref_switch
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_outside_ref_switch::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_outside_ref_switch::processCamDetected()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_outside_ref_switch::isCorrectSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_outside_ref_switch::setCorrectNextSlope()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_outside_ref_switch::isMiddleSwitchButFoundHighSlopeForCorrectEndSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_outside_ref_switch::changeDirection()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CStart_on_switch
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_on_switch::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_on_switch::isWrongSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_on_switch::isCorrectSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_on_switch::setCorrectNextSlope()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CEnd_switch_low_high
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CEnd_switch_low_high::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CEnd_switch_low_high::hasFoundAnotherEndSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CEnd_switch_low_high::setError()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CEnd_switch_low_high::processCamDetected()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CEnd_switch_low_high::hasFoundCorrectSlope() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CCorrect_switch_fast_low_high
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CCorrect_switch_fast_low_high::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_fast_low_high::notFoundCorrectSlope() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CCorrect_switch_fast_low_high::setError()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CCorrect_switch_fast_low_high::processCamDetected()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_fast_low_high::foundCorrectSlope() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CStart_slow_motion
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_slow_motion::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_slow_motion::notFoundCorrectSlope() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_slow_motion::setError()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CStart_slow_motion::processCamDetected()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CStart_slow_motion::foundCorrectSlope() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CCorrect_switch_high_low
    // ***********************************************************************


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_high_low::isMiddleSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CCorrect_switch_high_low::changeDirection()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_high_low::isZmAndEndSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_high_low::isNotZmAndEndSwitch() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CCorrect_switch_slow_low_high
    // ***********************************************************************


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_slow_low_high::isZmActive() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CCorrect_switch_slow_low_high::isZmNotActive() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CSearch_for_zm
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSearch_for_zm::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSearch_for_zm::setError()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSearch_for_zm::processZmSignal()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CSearch_for_zm::isK0Detected() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }



    // ***********************************************************************
    //                            CSet_reference_point
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSet_reference_point::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CSet_reference_point::isNotComplete() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSet_reference_point::setError()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CSet_reference_point::isComplete() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSet_reference_point::setReference()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CExternal
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CExternal::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CExternal::readDriveData()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CExternal::isComplete() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CExternal::reportEndOfMotionHomeExternal()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CSend_reference_data_to_drive
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSend_reference_data_to_drive::enter()
    {
        // place your code here...
    }


    //************************************************************************
    //! \brief         
    //! \retval        
    //************************************************************************
    bool MotionHomeSM::CSend_reference_data_to_drive::hasDriveAnswered() const
    {
        // place your code here...

        // this return is here just to silence the compiler errors/warnings
        return false;
    }


    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CSend_reference_data_to_drive::reportEndOfMotionHome()
    {
        // place your code here...
    }



    // ***********************************************************************
    //                            CError
    // ***********************************************************************

    //************************************************************************
    //! \brief         
    //************************************************************************
    void MotionHomeSM::CError::enter()
    {
        // place your code here...
    }

} // namespace MotionHome

