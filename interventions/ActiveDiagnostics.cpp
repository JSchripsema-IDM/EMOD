/***************************************************************************************************

Copyright (c) 2015 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode.

***************************************************************************************************/

#include "stdafx.h"
#include "ActiveDiagnostics.h"
#include "InterventionEnums.h"
#include "InterventionFactory.h"
#include "NodeEventContext.h"  // for INodeEventContext (ICampaignCostObserver)
#include "TBContexts.h"

static const char * _module = "ActiveDiagnostic";

namespace Kernel
{
    BEGIN_QUERY_INTERFACE_DERIVED(ActiveDiagnostic, SimpleDiagnostic)
    END_QUERY_INTERFACE_DERIVED(ActiveDiagnostic, SimpleDiagnostic)

    IMPLEMENT_FACTORY_REGISTERED(ActiveDiagnostic)

    bool ActiveDiagnostic::Configure(
        const Configuration * inputJson
    )
    {
        return SimpleDiagnostic::Configure(inputJson);
    }

    ActiveDiagnostic::ActiveDiagnostic() : SimpleDiagnostic()
    {
        initSimTypes( 1, "TB_SIM" );

    }
    ActiveDiagnostic::~ActiveDiagnostic()
    { 
        LOG_DEBUG("Destructing Active Diagnostic \n");
    }

    bool ActiveDiagnostic::positiveTestResult()
    {
        LOG_DEBUG("Positive test Result function\n");

        // Apply diagnostic test with given specificity/sensitivity
        float rand = parent->GetRng()->e();

        IIndividualHumanTB* tb_ind = NULL;
        if(parent->QueryInterface( GET_IID( IIndividualHumanTB ), (void**)&tb_ind ) != s_OK)
        {
            throw QueryInterfaceException( __FILE__, __LINE__, __FUNCTION__, "parent", "IIndividualHumanTB", "IIndividualHuman" );
        }
        bool activeinf = tb_ind->HasActiveInfection();

        // always return negative if the person is not infected, intended to be used with GroupEventCoordinator
        // TODO: allow to distribute Smear diagnostic to non-infected individuals?

        bool positiveTest = false;
        // True positive (sensitivity), or False positive (1-specificity)
        positiveTest = ( activeinf && (rand < base_sensitivity) ) || ( !activeinf && (rand > base_specificity) );
        return positiveTest;

    }


}