#include "scene.hpp"

void Scene::Save() {

    std::cout << "Here" << std::endl;
    std::filesystem::create_directories(scene_dir + m_name);
    SaveWorld();
    SaveCamera();
    SaveConfig();
}

void Scene::SaveWorld() {
    std::ofstream ofs;
    ofs.open(scene_dir + m_name + "/world.cfg");
    ofs << m_world;
    ofs.close();
}

void Scene::SaveCamera() {

}

void Scene::SaveConfig() {

}

void Scene::Load() {
    m_world.Clear();
    m_config.Clear();
    LoadWorld();
}

void Scene::LoadWorld() {

    std::ifstream ifs;
    std::filesystem::path path {scene_dir + m_name + "/world.cfg" };
    if (std::filesystem::exists(path)) {
        std::cout << "exists" << std::endl;
        ifs.open(path);
        ifs >> m_world;
        std::cout << m_world << std::endl;
    }
    std::vector<Body*> bodies = m_world.Bodies();
    int max_id = -1;
    int id;
    for (Body *body: bodies) {
        id = body->ID();
        // std::cout << "id:" << id << std::endl;

        if (id > max_id) {
            max_id = (int) id;
            // std::cout << "bigger:" << max_id << std::endl;
        }
    }
    std::cout << "max:" << max_id << std::endl;
    Body::SetIDCounter(++max_id);
    std::cout << Body::GetIDCounter() << std::endl;
    // std::cout << "got bodies" << std::endl;
    // for (Body *body: bodies)
        // std::cout << *body;
    // std::cout << "got bodies" << std::endl;

    m_config.RegisterBodies(bodies);
    // std::cout << "done loading" << std::endl;

}

void Scene::LoadCamera() {

}

void Scene::LoadConfig() {

}
