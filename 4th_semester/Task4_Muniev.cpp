#include <iostream>
#include <cstring>
#include <ctime>

class MobileApps // абстрактный класс
{
private:
    char *name = nullptr;
    int sizeMb;
public:
    virtual void launch() = 0; // чисто виртуальная функция для абстрактности класса
    MobileApps& operator=(const MobileApps &B);
    void setName (const char* src);
    void setSizeMb (const int size);
    void printInfo () const;
    ~MobileApps();
};

MobileApps& MobileApps::operator=(const MobileApps &B) // оператор = для абстрактного класса.
{
    if (this == &B) return *this;
    delete[] name;
    name = new char [strlen(B.name)+1];
    strcpy(name,B.name);
    sizeMb = B.sizeMb;
    return *this;
}


void MobileApps::setName(const char *src)
{
    if (name != nullptr) delete [] name; // если переименовываем, то удялем прошлое имя
    name = new char [strlen(src)+1];
    strcpy(name,src);
}

void MobileApps::setSizeMb(const int size)
{
    sizeMb = size;
}

void MobileApps::printInfo() const
{
    std::cout<< "App name: " << name << " | App size in megabytes: " << sizeMb << std::endl;
}

MobileApps::~MobileApps()
{
    delete [] name;
}

class TimeApps: public MobileApps
{
public:
    TimeApps();
    TimeApps(const char* src, const int size);
    void launch() override;
};

TimeApps::TimeApps()
{
    setSizeMb(0);
    setName("\0");
}
TimeApps::TimeApps(const char *src, const int size)
{
    setSizeMb(size);
    setName(src);
}

void TimeApps::launch()
{
    time_t now = time(0);
    tm* localtm = localtime(&now);
    std::cout << "The local date and time is: " << asctime(localtm);
}

class WeatherApps: public MobileApps
{
public:
    WeatherApps();
    WeatherApps(const char* src, const int size);
    void launch() override;
};
WeatherApps::WeatherApps()
{
    setSizeMb(0);
    setName("\0");
}
WeatherApps::WeatherApps(const char *src, const int size)
{
    setSizeMb(size);
    setName(src);
}

void WeatherApps::launch()
{
    std::cout << "The current weather is .......... " << std::endl;
}

class MusicApps: public MobileApps
{
private:
    int tracksPlayed; // счетчик запуска приложения
public:
    MusicApps();
    MusicApps(const char* src, const int size);
    void launch() override;
};

MusicApps::MusicApps()
{
    tracksPlayed = 0;
    setSizeMb(0);
    setName("\0");
}
MusicApps::MusicApps(const char *src, const int size)
{
    tracksPlayed = 0;
    setSizeMb(size);
    setName(src);
}

void MusicApps::launch()
{
    std::cout << "Playing music ♫ ♫ ♫ " << "| Tracks played: " << tracksPlayed << std::endl;
    tracksPlayed++;
}

class Phone
{
private:
    int appsSize; // количество приложений в телефоне ( количество элементов массива)
    MobileApps** app; // массив указателей
public:
    Phone();
    Phone(const int aSize);
    void launchAllApps();
    void setApp(const char* src, const int size, const int idx, const int type); // тип от 0 до 2, 0 - время, 1 - погода, 2 - музыка
    ~Phone();
};

Phone::Phone()
{
    appsSize = 1;
    app = new MobileApps*[appsSize];
}
Phone::Phone(const int aSize)
{
    appsSize = aSize;
    app = new MobileApps* [appsSize];
}
Phone::~Phone()
{
    for (int i=0; i<appsSize; ++i) // очищаем память массива указателей
    {
        delete app[i];
    }
    delete[] app;
}

void Phone::launchAllApps() // печатаает информацию о всех приложениях, которые находятся в массиве и запускает их
{
    for (int i=0; i<appsSize; ++i)
    {
        app[i]->printInfo();
        app[i]->launch();
    }
}

void Phone::setApp(const char *src, const int size, const int idx, const int type) // тип от 0 до 2, 0 - время, 1 - погода, 2 - музыка
{
    switch (type)
    {
        case 0:
            app[idx] = new TimeApps(src, size);
            break;
        case 1:
            app[idx] = new WeatherApps(src, size);
            break;
        case 2:
            app[idx] = new MusicApps(src, size);
            break;
        default:
            std::cout << "Incorrect type of app, try to enter 0-2 type" << std::endl;
    }
}
int main()
{
    TimeApps x1("Android default time", 2);
    x1.printInfo();
    x1.launch();
    MusicApps x2("Spotify",15);
    //x2.printInfo();
    //x2.launch();
    TimeApps x4;
    x4 = x1; // оператор = для объектов производного от абстрактного класса
    x4.printInfo(); // проверка работы
    x4.launch();
    std::cout<< std::endl;
    Phone iPhone(3); // создаем телефон и инициализируем массив указателей
    iPhone.setApp("Apple Music", 25, 0, 2);  // тип от 0 до 2, 0 - время, 1 - погода, 2 - музыка
    iPhone.setApp("Default time", 2, 1, 0);
    iPhone.setApp("Yandex weather", 10, 2, 1);
    iPhone.launchAllApps();
    std::cout<< std::endl;
    Phone Xiaomi(4); // создаем телефон и инициализируем массив указателей
    Xiaomi.setApp("Boom", 50,0,2);
    Xiaomi.setApp("Default Android time", 3, 1,0);
    Xiaomi.setApp("Google weather",10,2,1);
    Xiaomi.setApp("Clock", 12, 3,0);
    Xiaomi.launchAllApps();
    std::cout<< std::endl;
    iPhone.launchAllApps();
}