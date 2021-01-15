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
    // TODO check existence
    // TODO implement world.Clear();
    // m_world.Clear();
    std::ifstream ifs;
    ifs.open(scene_dir + m_name + "/world.cfg");
    ifs >> m_world;
    std::cout << m_world << std::endl;
}

void Scene::LoadWorld() {

}

void Scene::LoadCamera() {

}

void Scene::LoadConfig() {

}
