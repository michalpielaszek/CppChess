
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <wx/image.h>
#include <Piece.h>
#include <map>

class View;
class GameModel;

class Controller {

public:

    /*  Notification for myself
     *
     *  The class View; differs from #include "View.h", in subtle way.
     *  class View; says trust me there is a declaration, lets just go forward
     *  #include "View.h" provides full declaration of the class
     *  However if View need Controller and Controller needs View, full declaration is
     *  impossible to be provided. Thus, setController for both View and GameModel is provided.
     *  This way before anything is used, controller member is nullptr, quickly after
     *  declaration, controllers for both modules is initialized resolving the issue.
     *
     *  Concept learnt is forward declaration, where no precise class definition is provided,
     *  but only assurance that one will be ready later.
     *  Moreover, forward declaration should be outside class, in global scope.
     *  One placed inside the class makes it class member.
     */

private:
    View* view_;
    GameModel* model_;

public:
    Controller(View *view, GameModel *model) : view_(view), model_(model) {}

    std::map<PieceName, wxImage> piecesPiecesMap_;

    void initialize_pieces_images();
    void initialize_controller();

    void map_all_pieces_to_tiles();
};



#endif //CONTROLLER_H
