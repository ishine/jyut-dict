#include "windows/mainwindow.h"

#include "logic/settings/settings.h"

#include <QApplication>
#include <QSysInfo>

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <sapi.h>

#include <cstdio>
#include <cstring>

constexpr auto ARG_STR_LEN = 64;
constexpr auto platformArg = "-platform";
constexpr auto darkModeArg = "windows:darkmode=1";
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Aaron Tan");
    QCoreApplication::setOrganizationDomain("aaronhktan.com");
#ifdef APPIMAGE
    QCoreApplication::setApplicationName("Jyut Dictionary");
#else
    QCoreApplication::setApplicationName("CantoneseDictionary");
#endif

    // Prevents some UI lag
    QThreadPool::globalInstance()->setMaxThreadCount(16);

    // Required to marshal types to QVariant, so these values can be stored
    // by QSettings or used in a QListModel
    qRegisterMetaType<EntryCharactersOptions>("EntryCharactersOptions");
    qRegisterMetaType<EntryPhoneticOptions>("EntryPhoneticOptions");
    qRegisterMetaType<CantoneseOptions>("CantoneseOptions");
    qRegisterMetaType<MandarinOptions>("MandarinOptions");
    qRegisterMetaType<EntryColourPhoneticType>("EntryColourPhoneticType");
    qRegisterMetaType<SearchParameters>("SearchParameters");
#ifndef Q_OS_LINUX
    qRegisterMetaType<TranscriptionLanguage>("TranscriptionLanguage");
#endif
    qRegisterMetaType<searchTermHistoryItem>("searchTermHistoryItem");
    qRegisterMetaType<conflictingDictionaryMetadata>(
        "conflictingDictionaryNamesMetadata");
    qRegisterMetaType<Settings::InterfaceSize>();
    qRegisterMetaType<TextToSpeech::SpeakerBackend>();
    qRegisterMetaType<TextToSpeech::SpeakerVoice>();

#if defined(Q_OS_WIN)
    // This is kind of a horrible hack to get dark borders on Windows
    // But it works!
    int new_argc = argc + 2;
    char **new_argv = static_cast<char **>(
        malloc(sizeof(*new_argv) * (argc + 2)));
    memcpy(new_argv, argv, argc * sizeof(*argv));
    char new_platform_arg[ARG_STR_LEN];
    snprintf(new_platform_arg, ARG_STR_LEN, "%s", platformArg);
    new_argv[argc] = new_platform_arg;
    char new_dark_mode_arg[ARG_STR_LEN];
    snprintf(new_dark_mode_arg, ARG_STR_LEN, "%s", darkModeArg);
    new_argv[argc + 1] = new_dark_mode_arg;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return -1;
    }

    QApplication a{new_argc, new_argv};
#else
    QApplication a{argc, argv};
#endif

    MainWindow w;
    w.show();

    int res = a.exec();

#ifdef Q_OS_WIN
    CoUninitialize();
#endif

    return res;
}
