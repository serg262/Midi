#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <mmsystem.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    HMIDIOUT Out = nullptr;                 // Ключ устройства вывода
    int Chan = 0;                    // Номер канала

    // Посылка сообщения на открытое устройство
    void MidiOut (DWORD Msg);

    // Посылка канального сообщения
    void MidiOutChan (BYTE b1, BYTE b2, BYTE b3);

    // Посылка сообщения о смене тембра
    void SendPatchChange (int patch);

    // Посылка сообщения "All Notes Off"
    void AllNotesOff (void);

    // Отработка смены тембра
    void PatchChange (int patch);

    // Закрытие устройств
    void CloseDevices (void);

    // Открытие устройств
    int OpenDevices (void);

    void write_setting();
    void read_setting();
    void play_note(int note, int chan, int velocity);


protected:

    bool eventFilter( QObject *obj, QEvent *event ) override;

private slots:

    void on_comboBox_patch_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
