#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSettings>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QPushButton*> key_list = this->findChildren<QPushButton*>();
    foreach(QPushButton* key, key_list)
    key->installEventFilter(this);

    QStringList list;
    list << "Acoustic Grand Piano";
    list << "Bright Acoustic Piano";
    list << "Electric Grand Piano";
    list << "Honky-tonk Piano";
    list << "Electric Piano 1";
    list << "Electric Piano 2";
    list << "Harpsichord";
    list << "Clavinet";
    list << "Celesta";
    list << "Glockenspiel";
    list << "Music Box";
    list << "Vibraphone";
    list << "Marimba";
    list << "Xylophone";
    list << "Tubular Bells";
    list << "Dulcimer";
    list << "Drawbar Organ";
    list << "Percussive Organ";
    list << "Rock Organ";
    list << "Church Organ";
    list << "Reed Organ";
    list << "Accordion";
    list << "Harmonica";
    list << "Tango Accordion";
    list << "Acoustic Guitar (nylon)";
    list << "Acoustic Guitar (steel)";
    list << "Electric Guitar (jazz)";
    list << "Electric Guitar (clean)";
    list << "Electric Guitar (muted)";
    list << "Overdriven Guitar";
    list << "Distortion Guitar";
    list << "Guitar Harmonics";
    list << "Acoustic Bass";
    list << "Electric Bass (finger)";
    list << "Electric Bass (pick)";
    list << "Fretless Bass";
    list << "Slap Bass 1";
    list << "Slap Bass 2";
    list << "Synth Bass 1";
    list << "Synth Bass 2";
    list << "Violin";
    list << "Viola";
    list << "Cello";
    list << "Contrabass";
    list << "Tremolo Strings";
    list << "Pizzicato Strings";
    list << "Orchestral Harp";
    list << "Timpani";
    list << "String Ensemble 1";
    list << "String Ensemble 2";
    list << "Synth Strings 1";
    list << "Synth Strings 2";
    list << "Choir Aahs";
    list << "Voice Oohs";
    list << "Synth Voice";
    list << "Orchestra Hit";
    list << "Trumpet";
    list << "Trombone";
    list << "Tuba";
    list << "Muted Trumpet";
    list << "French Horn";
    list << "Brass Section";
    list << "Synth Brass 1";
    list << "Synth Brass 2";
    list << "Soprano Sax";
    list << "Alto Sax";
    list << "Tenor Sax";
    list << "Baritone Sax";
    list << "Oboe";
    list << "English Horn";
    list << "Bassoon";
    list << "Clarinet";
    list << "Piccolo";
    list << "Flute";
    list << "Recorder";
    list << "Pan Flute";
    list << "Bottle Blow";
    list << "Shakuhachi";
    list << "Whistle";
    list << "Ocarina";
    list << "Lead 1 (square)";
    list << "Lead 2 (sawtooth)";
    list << "Lead 3 (calliope)";
    list << "Lead 4 (chiff)";
    list << "Lead 5 (charang)";
    list << "Lead 6 (voice)";
    list << "Lead 7 (fifths)";
    list << "Lead 8 (bass + lead)";
    list << "Pad 1 (new age)";
    list << "Pad 2 (warm)";
    list << "Pad 3 (polysynth)";
    list << "Pad 4 (choir)";
    list << "Pad 5 (bowed)";
    list << "Pad 6 (metallic)";
    list << "Pad 7 (halo)";
    list << "Pad 8 (sweep)";
    list << "FX 1 (rain)";
    list << "FX 2 (soundtrack)";
    list << "FX 3 (crystal)";
    list << "FX 4 (atmosphere)";
    list << "FX 5 (brightness)";
    list << "FX 6 (goblins)";
    list << "FX 7 (echoes)";
    list << "FX 8 (sci-fi)";
    list << "Sitar";
    list << "Banjo";
    list << "Shamisen";
    list << "Koto";
    list << "Kalimba";
    list << "Bagpipe";
    list << "Fiddle";
    list << "Shanai";
    list << "Tinkle Bell";
    list << "Agogo";
    list << "Steel Drums";
    list << "Woodblock";
    list << "Taiko Drum";
    list << "Melodic Tom";
    list << "Synth Drum";
    list << "Reverse Cymbal";
    list << "Guitar Fret Noise";
    list << "Breath Noise";
    list << "Seashore";
    list << "Bird Tweet";
    list << "Telephone Ring";
    list << "Helicopter";
    list << "Applause";
    list << "Gunshot";

    ui->comboBox_patch->addItems(list);

    read_setting();

    OpenDevices();       // Откроем новое устройство
}

MainWindow::~MainWindow()
{
    CloseDevices();
    write_setting();
    delete ui;
}

void MainWindow::MidiOut(DWORD Msg) {

    if (Out) {

        midiOutShortMsg (Out, Msg);

    }

}

void MainWindow::MidiOutChan(BYTE b1, BYTE b2, BYTE b3) {


    MidiOut ((((b3 << 8) | b2) << 8) | (b1 | BYTE (Chan)));

}

void MainWindow::SendPatchChange(int patch) {

    MidiOutChan (0xC0, BYTE (patch), 0); // Сообщение структуры Cn pp

}

void MainWindow::AllNotesOff() {

    MidiOutChan (0xB0, 123, 0);   // Сообщение структуры Bn 7B

}

void MainWindow::PatchChange(int patch) {

    if(!Out)return;

    PATCHARRAY Patches;           // Массив включенных тембров

    for (int i = 0; i < MIDIPATCHSIZE; i++) {

        Patches [i] = 0;            // Обнуляем массив тембров

    }

    Patches [patch] = 0xFFFF;     // Включим тембр для всех каналов

    midiOutCachePatches (Out, 0, Patches, MIDI_CACHE_ALL); // Загрузим тембры

    SendPatchChange (patch);           // Пошлем сообщение о смене тембра

}

void MainWindow::CloseDevices() {

    if (Out) {

        midiOutReset (Out);         // Сбросим активность

        AllNotesOff ();             // Пошлем сообщение сброса нот

        Sleep (10);                 // Подождем, пока уйдет

        midiOutClose (Out);         // Закроем устройство

        Out = nullptr;

    }

}

int MainWindow::OpenDevices() {

    MMRESULT Res;

    Res = midiOutOpen (&Out, 0, 0, 0, CALLBACK_NULL);

    if(Res != MMSYSERR_NOERROR)
    {

        qDebug()<< "Невозможно открыть устройство вывода";
        return FALSE;

    }
    PatchChange(ui->comboBox_patch->currentIndex());// Выберем текущий тембр

    return TRUE;

}

void MainWindow::write_setting()
{
    QSettings qsettings( "home", "settingsapp" );

        qsettings.beginGroup( "midi" );

       // qsettings.setValue("geometry", this->saveGeometry());
        qsettings.setValue("voice", ui->comboBox_patch->currentIndex());

        qsettings.endGroup();

}
void MainWindow::read_setting()
{

    QSettings qsettings( "home", "settingsapp" );

        qsettings.beginGroup( "midi" );

       // this->restoreGeometry(qsettings.value("geometry", this->saveGeometry() ).toByteArray());

        ui->comboBox_patch->setCurrentIndex(qsettings.value("voice", false).toInt());

        qsettings.endGroup();

}

void MainWindow::play_note(int note, int chan, int velocity)
{
    qint32 msg = 0x90 + chan;
    msg |= note << 8;
    msg |= velocity << 16;

    midiOutShortMsg(Out, DWORD(msg));
}


bool MainWindow::eventFilter( QObject *obj, QEvent *event )
{

    if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
    {
    int note;

    QStringList list;

    QPushButton *button = qobject_cast<QPushButton*>(obj);
    list = button->objectName().split("_");
    note = list.at(1).toInt();

    switch (int(event->type()))
    {
    case QEvent::MouseButtonPress:
    play_note(note, int(Chan), 127);
    break;

    case QEvent::MouseButtonRelease:
    play_note(note, int(Chan), 0);
    break;
    }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_comboBox_patch_currentIndexChanged(int index)
{
    PatchChange(index);
}
