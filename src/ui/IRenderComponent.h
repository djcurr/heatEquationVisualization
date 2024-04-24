//
// Created by djcur on 4/19/2024.
//

#ifndef IRENDERCOMPONENT_H
#define IRENDERCOMPONENT_H
class IRenderComponent {
public:
    virtual ~IRenderComponent() = default;
    virtual void render() = 0;
};

#endif //IRENDERCOMPONENT_H
