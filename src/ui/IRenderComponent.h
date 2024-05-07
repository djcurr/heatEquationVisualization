//
// Created by djcur on 4/19/2024.
//

#ifndef IRENDERCOMPONENT_H
#define IRENDERCOMPONENT_H
/**
 * @class IRenderComponent
 * @brief Interface for components that provide rendering functionality.
 *
 * This interface ensures that all renderable components in the system conform to a standard
 * method of operation, specifically providing a render function that encapsulates the component's
 * rendering logic. Implementing this interface allows components to be used interchangeably
 * within the rendering framework of the application.
 */
class IRenderComponent {
public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures that derived classes can be cleaned up correctly through base class pointers,
     * preventing resource leaks.
     */
    virtual ~IRenderComponent() = default;

    /**
     * @brief Render the component.
     *
     * This function is called to execute the rendering operations specific to the component.
     * Implementations of this interface must define this function to handle the drawing of
     * the component's visuals.
     */
    virtual void render() = 0;
};

#endif //IRENDERCOMPONENT_H
