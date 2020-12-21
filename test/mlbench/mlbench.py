import argparse
import urllib
import urllib.request

import numpy as np
from tvm.driver import tvmc

def run():
    # tuning_opt is removed since it is not used
    name_list = ['mobilenet_v3.tar', 'ant_model.tar']
    url_list = ['http://nyx.skku.ac.kr/starlab/ml/mobilenet_v3.tar',
                'http://nyx.skku.ac.kr/starlab/ml/ant_model.tar']

    for name, url in zip(name_list, url_list):
        urllib.request.urlretrieve(url, name)
        _, times = tvmc.runner.run_module(
            name,
            args.host,
            port=args.port,
            rpc_key=args.device_key,
            device='cpu',
            inputs_file=None,
            fill_mode="random",
            repeat=1,
            profile=False
        )

        # evaluate
        prof_res = np.mean(times) * 1000
        print(
            "Mean inference time: %.2f ms"
            % (np.mean(prof_res))
        )


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--host', default='115.145.179.79', type=str)
    parser.add_argument('--port', default=9090, type=int)
    parser.add_argument('--device-key', default='xu4', type=str)

    args = parser.parse_args()

    run(args)
