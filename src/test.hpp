void TestWorld() {
    World w, w2;
    w.AddBody("A");
    w.AddBody("B");
    w.AddBody("C");
    w.AddBody("D");
    w.AddBody("E");
    w.AddBody("F");


    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << w;
    ofs.close();

    ifs.open("test.txt");
    ifs >> w2;
    std::cout << "printing world " <<  w2.GetCount() << std::endl;
    std::cout << w2;
}

void TestCamera() {

    Camera camera;
    std::cout << camera;
    camera.SetCenter(glm::vec3(10.0f, 20.0f, 0.0f));
    std::cout << camera;

    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << camera;
    ofs.close();

    ifs.open("test.txt");
    ifs >> camera;
    std::cout << camera;
}

void TestBody() {
    Body ba("A");
    Body bb;

    std::ofstream ofs;
    std::ifstream ifs;
    ofs.open("test.txt");
    ofs << ba;
    ofs.close();

    ifs.open("test.txt");
    ifs >> bb;
    std::cout << bb;
}
