#ifndef CITRORENDERER_H
#define CITRORENDERER_H
class CitroRenderer {
    public:
    CitroRenderer();
    ~CitroRenderer();
    void make_current();
};

CitroRenderer *new_CitroRenderer();
#endif