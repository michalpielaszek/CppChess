#include <GameModel.h>
#include "View.h"
#include "Controller.h"
#include <filesystem>
#include <wx/filename.h>
#include <wx/stdpaths.h>

int main(int argc, char** argv) {
    std::cout << "Current working dir: " << std::filesystem::current_path() << std::endl;

    // Create View, Model, Controller
    View* view = new View();
    GameModel* model = new GameModel();
    Controller* controller = new Controller(view, model);

    // Set references
    view->setController(controller);
    model->setController(controller);

    // Start wxWidgets (calls View::OnInit internally)

    return View::start(argc, argv, *view);
}

