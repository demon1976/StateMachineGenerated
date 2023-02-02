//! \file *****************************************************************
//! ***********************************************************************
//!  Module : MC_Kernel
//! ***********************************************************************
//! *
//! * The following comments will be automatically
//! * inserted by the source control tool:
//! *
//! * $HeadURL: http://svnserver/svn/modules/MC_Kernel/tags/trunk/src/MotionHomeSM_DoNotChange.cpp $
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

    // c-tor MotionHomeSM
    MotionHomeSM::MotionHomeSM()
    : mRoot(this)
    , mWorking(this)
    , mStart_finish(this)
    , mMotion_home_ongoing(this)
    , mInit(this)
    , mStart_outside_ref_switch(this)
    , mStart_on_switch(this)
    , mEnd_switch_low_high(this)
    , mCorrect_switch_fast_low_high(this)
    , mStart_slow_motion(this)
    , mCorrect_switch_high_low(this)
    , mCorrect_switch_slow_low_high(this)
    , mSearch_for_zm(this)
    , mSet_reference_point(this)
    , mExternal(this)
    , mSend_reference_data_to_drive(this)
    , mError(this)
    , isCurrentDirectionPositive(false)
    , isStartDirectionPositive(false)
    , hasReferenceSwitch(false)
    , hasEndSwitches(false)
    , kindOfReferenceMark(UNDEFINED_REF_MARK)
    , numberOfPhases(UNDEFINED_PHASE_NUMBER)
    , acceleration(0.0)
    , speedDuringSearchForSwitch(0.0)
    , maxDistanceSearchingForSwitch(0.0)
    , speedDuringSearchForReference(0.0)
    , maxDistanceSearchingForReference(0.0)
    , nullPointDistance(0.0)
    , nullPointPosition(0.0)
    , referenceMark(DOUBLE.MAX_VALUE)
    , isComplete(false)
    , isError(false)
    , errorType(UNDEFINED_ERROR_TYPE)
    , isMotionHomeStarted(false)
    , lastKindOfSlope(UNDEFINED_KIND_OF_SLOPE)
    , isMotionHomeSet(false)
    , isUpdateOfMotionHomeDataDesired(false)
    , isUserHalt(false)
    , isAxisStopped(false)
    , isK0Detected(false)
    , isCamDetected(false)
    {
    }

    // destructor MotionHomeSM
    MotionHomeSM::~MotionHomeSM()
    {
    }

    // processEvent => just forward to root state
    void MotionHomeSM::processEvent(const EventID eMessage)
    {
        preProcess();
        (void) mRoot.dispatch(eMessage);
        postProcess();
    }

    // switchState
    void MotionHomeSM::switchState(CGeneralState& from, CGeneralState& to)
    {
        CGeneralState* tmpExit  = &from;
        CGeneralState* tmpEnter = &to;

        CGeneralState* commonFatherState = &(from.findCommonFatherWith(to));

        // exit all states until common super state of "from" and "to"
        while(tmpExit != commonFatherState)
        {
            tmpExit->exit();
            tmpExit = tmpExit->getSuperState();
        }

        CGeneralState* tmpSuperEnter = tmpEnter->getSuperState();

        /* change the pointer "currentSubState" for all states between common super state and "to" */
        while(tmpSuperEnter != commonFatherState)
        {
            tmpSuperEnter->setCurrentSubState(*tmpEnter);
            tmpEnter      = tmpSuperEnter;
            tmpSuperEnter = tmpSuperEnter->getSuperState();
        }
        commonFatherState->setCurrentSubState(*tmpEnter);

        // enter all states between "common super state" and "to"
        tmpEnter = commonFatherState->getCurrentSubState();
        while(&to != tmpEnter)
        {
            tmpEnter->enter();
            tmpEnter = tmpEnter->getCurrentSubState();
        }

        // enter and update states for all states between "to" and a current leaf state
        // depend on kind of state (with history or without) this function adapts the
        // pointer to current sub state
        do
        {
            tmpEnter->enter();
            tmpEnter->updateSubState();
            tmpEnter = tmpEnter->getCurrentSubState();
        } while(0 != tmpEnter);
    }

    // ***********************************************************************
    //                            CGeneralState
    // ***********************************************************************

    //c-tor
    MotionHomeSM::CGeneralState::CGeneralState(MotionHomeSM* fsm, CGeneralState* f_superState, bool withHistory)
    : m_superState(f_superState)
    , m_currentSubState(0)
    , m_startSubState(0)
    , m_fsm(fsm)
    , m_withHistory_b(withHistory)
    {
    }

    //destructor
    MotionHomeSM::CGeneralState::~CGeneralState()
    {
    }

    int MotionHomeSM::CGeneralState::getDepth() const
    {
        int result = 0;
        const CGeneralState* curState = this;
        while(0 != curState)
        {
            curState = curState->m_superState;
            ++result;
        }
        return result;
    }

    MotionHomeSM::CGeneralState& MotionHomeSM::CGeneralState::findCommonFatherWith(CGeneralState& other) const
    {
        /*
         * to find it we have to compare first of all the depth of states
         */
        int depthMe    = this->getDepth();
        int depthOther = other.getDepth();

        MotionHomeSM::CGeneralState* tmpState4Me    = const_cast<MotionHomeSM::CGeneralState*>(this);
        MotionHomeSM::CGeneralState* tmpState4Other = &other;

        // if the depth is different...
        if (depthMe != depthOther)
        {
            // swap the sates to use common algorithm
            if(depthMe < depthOther) // swap
            {
                MotionHomeSM::CGeneralState* tmp =tmpState4Me;
                tmpState4Me    = tmpState4Other;
                tmpState4Other = tmp;
            }

            // go to super states as long as the depth is unequal
            for (int i = depthMe; i != depthOther; --i)
            {
                tmpState4Me = tmpState4Me->m_superState;
            }
        }

        // now both depths are equal -> go upstairs until the super states are equal.
        // This state is the common super state!
        while((tmpState4Me != tmpState4Other))
        {
            tmpState4Me    = tmpState4Me->m_superState;
            tmpState4Other = tmpState4Other->m_superState;
        }

        return *tmpState4Me;
    }

    MotionHomeSM::CGeneralState* MotionHomeSM::CGeneralState::getSuperState() const
    {
        return this->m_superState;
    }

    MotionHomeSM::CGeneralState* MotionHomeSM::CGeneralState::getCurrentSubState() const
    {
        return this->m_currentSubState;
    }

    void MotionHomeSM::CGeneralState::setCurrentSubState(CGeneralState& arg)
    {
        this->m_currentSubState = &arg;
    }

    void MotionHomeSM::CGeneralState::updateSubState()
    {
        if(false == this->m_withHistory_b)
        {
            this->m_currentSubState = this->m_startSubState;
        }
    }



    // ***********************************************************************
    //                            CRoot
    // ***********************************************************************

    //c-tor
    MotionHomeSM::CRoot::CRoot(MotionHomeSM* fsm)
    : CGeneralState(fsm, 0, false)
    {
        m_currentSubState = &(fsm->mWorking);
        m_startSubState = &(fsm->mWorking);
    }

    //destructor
    MotionHomeSM::CRoot::~CRoot()
    {
    }

    bool MotionHomeSM::CRoot::dispatch(const EventID eMessage)
    {
        return m_currentSubState->dispatch(eMessage);
    }

    const char* MotionHomeSM::CRoot::toString() const
    {
        return "FSM_ROOT";
    }



    // ***********************************************************************
    //                            CWorking
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CWorking::CWorking(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mRoot), false)
    {
        m_currentSubState = &(fsm->mStart_finish);
        m_startSubState = &(fsm->mStart_finish);
    }

    // destructor
    MotionHomeSM::CWorking::~CWorking()
    {
    }

    // dispatch
    bool MotionHomeSM::CWorking::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isErrorOccured())
                {
                    m_fsm->switchState(*this, m_fsm->mError);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => forward processing to sub state
                    return m_currentSubState->dispatch(eMessage);
                }
            }
            break;
            default:
            {
                // none of conditions applies => forward processing to sub state
                return m_currentSubState->dispatch(eMessage);
            }
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CWorking::toString() const
    {
        return "WORKING";
    }

    // ***********************************************************************
    //                            CStart_finish
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CStart_finish::CStart_finish(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mWorking), false)
    , isStarted(false)
    , isSet(false)
    , isExternal(false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CStart_finish::~CStart_finish()
    {
    }

    // dispatch
    bool MotionHomeSM::CStart_finish::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case AXIS_USER_REF_SET:
            {
                processSetMotionHome();
            }
            break;
            case AXIS_USER_REF_START:
            {
                processStartMotionHome();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(isExternalAxisAndMotionHomeActive())
                {
                    m_fsm->switchState(*this, m_fsm->mExternal);
                    return true;
                } // if|else if
                else if(isMotionHomeActiveAndAxisHold())
                {
                    startMotionHome();
                    m_fsm->switchState(*this, m_fsm->mMotion_home_ongoing);
                    return true;
                } // if|else if
                else if(isMotionHomeActiveButAxisInMotion())
                {
                    processHoldOn();
                } // if|else if
                else if(isSetReferenceActive())
                {
                    m_fsm->switchState(*this, m_fsm->mSend_reference_data_to_drive);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            case PARAM_UPDATE_HOMING:
            {
                updateParameters();
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CStart_finish::toString() const
    {
        return "START_FINISH";
    }

    // ***********************************************************************
    //                            CMotion_home_ongoing
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CMotion_home_ongoing::CMotion_home_ongoing(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mWorking), false)
    {
        m_currentSubState = &(fsm->mInit);
        m_startSubState = &(fsm->mInit);
    }

    // destructor
    MotionHomeSM::CMotion_home_ongoing::~CMotion_home_ongoing()
    {
    }

    // dispatch
    bool MotionHomeSM::CMotion_home_ongoing::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case AXIS_STOPPED:
            {
                processAxisStopped();
            }
            break;
            case AXIS_USER_REF_HALT:
            {
                processHalt();
            }
            break;
            default:
            {
                // none of conditions applies => forward processing to sub state
                return m_currentSubState->dispatch(eMessage);
            }
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CMotion_home_ongoing::toString() const
    {
        return "MOTION_HOME_ONGOING";
    }

    // ***********************************************************************
    //                            CInit
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CInit::CInit(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CInit::~CInit()
    {
    }

    // dispatch
    bool MotionHomeSM::CInit::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isZmOnly())
                {
                    startVzm();
                    m_fsm->switchState(*this, m_fsm->mSearch_for_zm);
                    return true;
                } // if|else if
                else if(isOnePhase())
                {
                    m_fsm->switchState(*this, m_fsm->mStart_slow_motion);
                    return true;
                } // if|else if
                else if(isOffSwitchAndTwoPhase())
                {
                    setCorrectSlope();
                    m_fsm->switchState(*this, m_fsm->mStart_outside_ref_switch);
                    return true;
                } // if|else if
                else if(isOnSwitchAndTwoPhase())
                {
                    m_fsm->switchState(*this, m_fsm->mStart_on_switch);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CInit::toString() const
    {
        return "INIT";
    }

    // ***********************************************************************
    //                            CStart_outside_ref_switch
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CStart_outside_ref_switch::CStart_outside_ref_switch(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CStart_outside_ref_switch::~CStart_outside_ref_switch()
    {
    }

    // dispatch
    bool MotionHomeSM::CStart_outside_ref_switch::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CAM_DETECTED:
            {
                processCamDetected();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(isCorrectSwitch())
                {
                    setCorrectNextSlope();
                    m_fsm->switchState(*this, m_fsm->mCorrect_switch_fast_low_high);
                    return true;
                } // if|else if
                else if(isMiddleSwitchButFoundHighSlopeForCorrectEndSwitch())
                {
                    changeDirection();
                    m_fsm->switchState(*this, m_fsm->mEnd_switch_low_high);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CStart_outside_ref_switch::toString() const
    {
        return "START_OUTSIDE_REF_SWITCH";
    }

    // ***********************************************************************
    //                            CStart_on_switch
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CStart_on_switch::CStart_on_switch(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CStart_on_switch::~CStart_on_switch()
    {
    }

    // dispatch
    bool MotionHomeSM::CStart_on_switch::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isWrongSwitch())
                {
                    m_fsm->switchState(*this, m_fsm->mStart_outside_ref_switch);
                    return true;
                } // if|else if
                else if(isCorrectSwitch())
                {
                    setCorrectNextSlope();
                    m_fsm->switchState(*this, m_fsm->mCorrect_switch_fast_low_high);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CStart_on_switch::toString() const
    {
        return "START_ON_SWITCH";
    }

    // ***********************************************************************
    //                            CEnd_switch_low_high
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CEnd_switch_low_high::CEnd_switch_low_high(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CEnd_switch_low_high::~CEnd_switch_low_high()
    {
    }

    // dispatch
    bool MotionHomeSM::CEnd_switch_low_high::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CAM_DETECTED:
            {
                processCamDetected();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(hasFoundAnotherEndSwitch())
                {
                    setError();
                } // if|else if
                else if(hasFoundCorrectSlope())
                {
                    m_fsm->switchState(*this, m_fsm->mCorrect_switch_high_low);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CEnd_switch_low_high::toString() const
    {
        return "END_SWITCH_LOW_HIGH";
    }

    // ***********************************************************************
    //                            CCorrect_switch_fast_low_high
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CCorrect_switch_fast_low_high::CCorrect_switch_fast_low_high(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CCorrect_switch_fast_low_high::~CCorrect_switch_fast_low_high()
    {
    }

    // dispatch
    bool MotionHomeSM::CCorrect_switch_fast_low_high::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CAM_DETECTED:
            {
                processCamDetected();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(notFoundCorrectSlope())
                {
                    setError();
                } // if|else if
                else if(foundCorrectSlope())
                {
                    m_fsm->switchState(*this, m_fsm->mCorrect_switch_high_low);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CCorrect_switch_fast_low_high::toString() const
    {
        return "CORRECT_SWITCH_FAST_LOW_HIGH";
    }

    // ***********************************************************************
    //                            CStart_slow_motion
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CStart_slow_motion::CStart_slow_motion(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CStart_slow_motion::~CStart_slow_motion()
    {
    }

    // dispatch
    bool MotionHomeSM::CStart_slow_motion::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CAM_DETECTED:
            {
                processCamDetected();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(notFoundCorrectSlope())
                {
                    setError();
                } // if|else if
                else if(foundCorrectSlope())
                {
                    m_fsm->switchState(*this, m_fsm->mCorrect_switch_slow_low_high);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CStart_slow_motion::toString() const
    {
        return "START_SLOW_MOTION";
    }

    // ***********************************************************************
    //                            CCorrect_switch_high_low
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CCorrect_switch_high_low::CCorrect_switch_high_low(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CCorrect_switch_high_low::~CCorrect_switch_high_low()
    {
    }

    // dispatch
    bool MotionHomeSM::CCorrect_switch_high_low::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isMiddleSwitch())
                {
                    changeDirection();
                    m_fsm->switchState(*this, m_fsm->mStart_slow_motion);
                    return true;
                } // if|else if
                else if(isZmAndEndSwitch())
                {
                    m_fsm->switchState(*this, m_fsm->mSearch_for_zm);
                    return true;
                } // if|else if
                else if(isNotZmAndEndSwitch())
                {
                    m_fsm->switchState(*this, m_fsm->mSet_reference_point);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CCorrect_switch_high_low::toString() const
    {
        return "CORRECT_SWITCH_HIGH_LOW";
    }

    // ***********************************************************************
    //                            CCorrect_switch_slow_low_high
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CCorrect_switch_slow_low_high::CCorrect_switch_slow_low_high(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CCorrect_switch_slow_low_high::~CCorrect_switch_slow_low_high()
    {
    }

    // dispatch
    bool MotionHomeSM::CCorrect_switch_slow_low_high::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isZmActive())
                {
                    m_fsm->switchState(*this, m_fsm->mSearch_for_zm);
                    return true;
                } // if|else if
                else if(isZmNotActive())
                {
                    m_fsm->switchState(*this, m_fsm->mSet_reference_point);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CCorrect_switch_slow_low_high::toString() const
    {
        return "CORRECT_SWITCH_SLOW_LOW_HIGH";
    }

    // ***********************************************************************
    //                            CSearch_for_zm
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CSearch_for_zm::CSearch_for_zm(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CSearch_for_zm::~CSearch_for_zm()
    {
    }

    // dispatch
    bool MotionHomeSM::CSearch_for_zm::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case AXIS_STOPPED:
            {
                setError();
            }
            break;
            case CYCLIC_EVENT:
            {
                if(isK0Detected())
                {
                    m_fsm->switchState(*this, m_fsm->mSet_reference_point);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            case K0_DETECTED:
            {
                processZmSignal();
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CSearch_for_zm::toString() const
    {
        return "SEARCH_FOR_ZM";
    }

    // ***********************************************************************
    //                            CSet_reference_point
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CSet_reference_point::CSet_reference_point(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mMotion_home_ongoing), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CSet_reference_point::~CSet_reference_point()
    {
    }

    // dispatch
    bool MotionHomeSM::CSet_reference_point::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case AXIS_STOPPED:
            {
                if(isNotComplete())
                {
                    setError();
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            case CYCLIC_EVENT:
            {
                if(isComplete())
                {
                    setReference();
                    m_fsm->switchState(*this, m_fsm->mSend_reference_data_to_drive);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CSet_reference_point::toString() const
    {
        return "SET_REFERENCE_POINT";
    }

    // ***********************************************************************
    //                            CExternal
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CExternal::CExternal(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mWorking), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CExternal::~CExternal()
    {
    }

    // dispatch
    bool MotionHomeSM::CExternal::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(isComplete())
                {
                    reportEndOfMotionHomeExternal();
                    m_fsm->switchState(*this, m_fsm->mStart_finish);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            case DRIVE_READ_DATA:
            {
                readDriveData();
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CExternal::toString() const
    {
        return "EXTERNAL";
    }

    // ***********************************************************************
    //                            CSend_reference_data_to_drive
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CSend_reference_data_to_drive::CSend_reference_data_to_drive(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mWorking), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CSend_reference_data_to_drive::~CSend_reference_data_to_drive()
    {
    }

    // dispatch
    bool MotionHomeSM::CSend_reference_data_to_drive::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                if(hasDriveAnswered())
                {
                    reportEndOfMotionHome();
                    m_fsm->switchState(*this, m_fsm->mStart_finish);
                    return true;
                } // if|else if
                else
                {
                    // none of conditions applies => processing is done
                    return true;
                }
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CSend_reference_data_to_drive::toString() const
    {
        return "SEND_REFERENCE_DATA_TO_DRIVE";
    }

    // ***********************************************************************
    //                            CError
    // ***********************************************************************

    // c-tor
    MotionHomeSM::CError::CError(MotionHomeSM* fsm)
    : CGeneralState(fsm, &(fsm->mRoot), false)
    {
        m_currentSubState = 0;
        m_startSubState = 0;
    }

    // destructor
    MotionHomeSM::CError::~CError()
    {
    }

    // dispatch
    bool MotionHomeSM::CError::dispatch(const EventID eMessage)
    {
        switch(eMessage)
        {
            case CYCLIC_EVENT:
            {
                m_fsm->switchState(*this, m_fsm->mWorking);
                return true;
            }
            break;
            default:
            {
            }
            break;
        } // switch
        return true;
    } // dispatch

    // toString
    const char* MotionHomeSM::CError::toString() const
    {
        return "ERROR";
    }

}    // namespace MotionHome


