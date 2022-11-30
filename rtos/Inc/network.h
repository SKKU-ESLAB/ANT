#ifndef __ANT_ML_NETWORK_H__
#define __ANT_ML_NETWORK_H__

#include <runtime/ant_ml_runtime_api.h>

#define _NETWORK_INPUTS_COUNT_ (1)
#define _NETWORK_OUTPUTS_COUNT_ (1)
#define _NETWORK_ACTIVATION_BYTES_ (78006)

ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_create (
  const ant_ml_ptr weights,
  const ant_ml_ptr activations
);

ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_destroy ();

ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_run (
  ant_ml_tensor *inputs[],
  ant_ml_tensor *outputs[]
);
#endif /*__ANT_ML_NETWORK_H__*/
