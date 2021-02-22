//
// Created by yuriy on 2/10/21.
//

#ifndef LOAD_BALANCER_CONFIGURATOR_H
#define LOAD_BALANCER_CONFIGURATOR_H


namespace lb::helpers {
    class configurator {
    public:
        configurator();

        int thread_count();

    private:
        static const char *default_config_path_;

    };
}

#endif //LOAD_BALANCER_CONFIGURATOR_H
