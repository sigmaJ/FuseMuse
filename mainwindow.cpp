#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <iostream>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadZips(packetPath, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
    loadZips(driverModulePath, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
    loadZips(controlModulePath, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadZips(QString path, QString namePropName, QComboBox *selectorBox, QMap<QString, FMZipInfo>& map) {
    QString startDir = QDir::currentPath();
    std::cout << "Current Dir: " << QDir::currentPath().toStdString() << std::endl;
    QDir::setCurrent(path);
    std::cout << "New Dir: " << QDir::currentPath().toStdString() << std::endl;
    QFileInfoList files = QDir::current().entryInfoList();
    std::cout << "Count: " << files.length() << std::endl;

    for(QFileInfo f : files) {
        std::cout << "File: " << f.completeBaseName().toStdString() << '.' << f.suffix().toStdString() << std::endl;
        if(f.suffix() == "jar" || f.suffix() == "zip") {
            FMZipInfo i(f.absoluteFilePath());
            QString key = i["ZipFilePath"];
            int matchIndex = map.size();
            if(map.find(key) != map.end()) {
                matchIndex = selectorBox->findData(key);
                selectorBox->removeItem(matchIndex);
            }
            selectorBox->insertItem(matchIndex, i[namePropName], QVariant(key));
            map.insert(key, i);
        }
    }
    QDir::setCurrent(startDir);
}

void MainWindow::on_packetComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->packetComboBox->currentData().toString();
    std::cout << "Packet Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = packets[fpath];

    ui->packetDescBox->setText(info["packet_description"]);

    ui->addMelodicButton->setDisabled(info["can_play_melody"] != "true");
    ui->addHarmonicButton->setDisabled(info["can_play_harmony"] != "true");
    ui->addSupportingButton->setDisabled(info["can_play_support"] != "true");
}

void MainWindow::on_driverComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->driverComboBox->currentData().toString();
    std::cout << "Driver Module Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = driverModules[fpath];

    ui->driverModuleDescBox->setText(info["driver_description"]);
}


void MainWindow::on_controlComboBox_activated(int index)
{
    (void)index; //make compiler think it's used

    QString fpath = ui->controlComboBox->currentData().toString();
    std::cout << "Control Module Selected: " << fpath.toStdString() << std::endl;

    FMZipInfo info = controlModules[fpath];

    ui->controlModuleDescBox->setText(info["control_description"]);
}

void MainWindow::on_actionLoad_Instruments_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionLoad_Driver_Modules_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionLoad_Control_Modules_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::DontUseNativeDialog
                                                    | QFileDialog::DontResolveSymlinks);
    loadZips(dir, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

void MainWindow::on_addMelodicButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* next = new QTreeWidgetItem(tree, Qt::UserRole);
    next->setText(0, packets[ui->packetComboBox->currentData().toString()]["packet_name"]);
    next->setData(0, Qt::UserRole, QVariant(QString("TEST")));
}

void MainWindow::on_addHarmonicButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree before adding a Harmonic Packet");
        msgBox.exec();
    } else {
        QTreeWidgetItem* next = new QTreeWidgetItem(cur, Qt::UserRole);
        next->setText(0, packets[ui->packetComboBox->currentData().toString()]["packet_name"]);
        next->setData(0, Qt::UserRole, QVariant(QString("TEST")));
    }
    tree->expandAll();
}

void MainWindow::on_addSupportingButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree before adding a Supporting Packet");
        msgBox.exec();
    } else {
        QTreeWidgetItem* next = new QTreeWidgetItem(cur, Qt::UserRole);
        next->setText(0, packets[ui->packetComboBox->currentData().toString()]["packet_name"]);
        next->setData(0, Qt::UserRole, QVariant(QString("TEST")));
    }
    tree->expandAll();
}

void MainWindow::on_removeInstrumentButton_clicked()
{
    QTreeWidget* tree = ui->compositionTree;
    QTreeWidgetItem* cur = tree->currentItem();
    if(!cur) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please select a Packet in the composition tree");
        msgBox.exec();
    } else {
        int index = tree->indexOfTopLevelItem(cur);
        //qt tree items delete children when deleted, thankfully
        if(index != -1) {
            delete tree->takeTopLevelItem(index);
        } else {
            cur->parent()->removeChild(cur);
            delete cur;
        }
    }
}

void MainWindow::on_actionClear_Instruments_triggered()
{
    ui->packetComboBox->clear();
    loadZips(packetPath, "packet_name", ui->packetComboBox, packets);
    on_packetComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionClear_Driver_Modules_triggered()
{
    ui->packetComboBox->clear();
    loadZips(driverModulePath, "driver_name", ui->driverComboBox, driverModules);
    on_driverComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionClear_Control_Modules_triggered()
{
    ui->packetComboBox->clear();
    loadZips(controlModulePath, "control_name", ui->controlComboBox, controlModules);
    on_controlComboBox_activated(0); // force update of description box
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionClear_Instrument_Hierarchy_triggered()
{
    ui->compositionTree->clear();
}
