#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qtextedit.h>
#include<QTextEdit>
#include <QFileDialog>
#include<QTextStream>
#include<QFileDialog>
#include<QTextStream>
#include<QIODevice>
#include<QFile>
#include<QDir>
#include<QMessageBox>
#include<QDesktopServices>
#include<QProcess>
#include<QLineEdit>
#include<QScrollBar>
#include<QtGui>
#include<QtPlugin>
#include<QGraphicsDropShadowEffect>
#include<QDesktopWidget>
#include<QtPrintSupport/QPrinter>
#include<QtPrintSupport/QPrintDialog>
#include<QPaintDevice>
#include <QtGui>
#include <QGuiApplication>
#include<QtWidgets>
#include<qgl.h>
#include<QMediaPlayer>
#include "openglscene.h"
#include<QGraphicsVideoItem>

bool chk_flag=0;
qint32 selection = 0;
qint32 analysis_selection=0;
QRegExp rx ("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
QString stored_path,storer;
QString nisa_filename,abaqus_filename,ansys_filename,hyfem_filename,program;

QProcess *myproc ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*Main Window Functionality*/
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint);                                                                           //Set window with no title bar
    //this->setWindowFlags(Qt::FramelessWindowHint);                                                                           //Set a frameless window

     stored_path = QDir::currentPath();                                                                                      //Obtains the current directory path of Software
     storer=QDir::toNativeSeparators(stored_path);

     ui->GraphicsView->setScene(new OpenGLScene);
     ui->GraphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
     ui->GraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
     ui->GraphicsView->show();
     ui->GraphicsView->resize(ui->GraphicsView->width(), ui->GraphicsView->height());
     ui->GraphicsView->setMouseTracking(true);


     //QGraphicsView *graphicsView = new QGraphicsView(this);
    // graphicsView->setScene(scene);

      // w = QMainWindow

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*Main Window Functionality*/

//Close
void MainWindow::on_Close_clicked()
{
    qApp->exit();                                                                                                            //Closes the application
}

//Minimize
void MainWindow::on_Minimize_clicked()
{
    showMinimized();                                                                                                         //Minimizes the application
}

//Restore
void MainWindow::on_Restore_clicked()
{
    showNormal();                                                                                                            //Restores previous size
}

//Maximize
void MainWindow::on_Maximize_clicked()
{
    showMaximized();                                                                                                         //Maximises to fill Window Screen
}

//Move Window
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_nMouseClick_X_Coordinate = event->x();                                                                                 //Obtains the x-coordinate according to the screen
    m_nMouseClick_Y_Coordinate = event->y();                                                                                 //Obtains the y-coordinate according to the screen
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);                            //Moves the window as per required coordinates
}

/*Browse for required File*/


void MainWindow::on_Browser_clicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this,tr("Browse"),"/home", tr("All Files (*.*);;NISA (*.NIS);; ABAQUS (*.inp)"));//Opens the file dialog box and stores the filepath to the variable
    ui->BrowsePath->setText(file1Name);                                                                                         //using the ui main object,we invoke the text to be displayed on the label
}

/*View Functionality - Read-Only Mode*/

void MainWindow::on_View_clicked()
{
    if(ui->BrowsePath->text()!="")
    {
        ui->Alert->setText("View Mode");
        ui->Alert->updateGeometry();
        QString fileName=ui->BrowsePath->text();
        QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
        myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
        QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
        QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
        myFile.close();                                                                                                           //Close the open file
        chk_flag=0;
        ui->Input_Information->setPlainText(line);                                                                                //Display contents of the file
        ui->Input_Information->setReadOnly(true);
    }
    else
    {
        ui->Alert->setText("Please open a File first");                                                                            //When the user tries to open without selecting a file
    }
}

/*Edit Functionality - Read-Write Mode*/

void MainWindow::on_View_2_clicked()
{

    if(ui->BrowsePath->text()!="")
    {
        ui->Alert->setText("Edit Mode");
        QString fileName=ui->BrowsePath->text();
        QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
        myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
        QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
        QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
        myFile.close();                                                                                                             //Close the open file
        chk_flag=1;
        ui->Input_Information->setPlainText(line);                                                                                  //Display contents of the file
        ui->Input_Information->setReadOnly(false);
    }
    else
    {
        ui->Alert->setText("Please open a File first");                                                                          //When the user tries to open without selecting a file
    }
}

/*Search Functionality - Highlight Mode*/

void MainWindow::on_Search_clicked()
{
        QString searchString = ui->SearchText->text();                                                                           //Takes the string entered as a search criteria
        QTextDocument *document = ui->Input_Information->document();                                                             //The scope of search

        bool found = false;

        if (isFirstTime == false)
            document->undo();

        if (searchString.isEmpty())                                                                                               //if Search field is empty
        {
            QMessageBox::information(this, tr("Empty Search Field"),
                    "The search field is empty. Please enter a word and click Find.");
        }
        else                                                                                                                       //Searches the entire document and highlights the search term
        {

            QTextCursor highlightCursor(document);
            QTextCursor cursor(document);

            cursor.beginEditBlock();

            QTextCharFormat plainFormat(highlightCursor.charFormat());
            QTextCharFormat colorFormat = plainFormat;
            colorFormat.setForeground(Qt::red);

            while (!highlightCursor.isNull() && !highlightCursor.atEnd())
            {
                highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

                if (!highlightCursor.isNull())
                {
                    found = true;
                    highlightCursor.movePosition(QTextCursor::WordRight,QTextCursor::KeepAnchor);
                    highlightCursor.mergeCharFormat(colorFormat);
                }
            }

            cursor.endEditBlock();
            isFirstTime = false;

            if (found == false)
            {
                QMessageBox::information(this, tr("Search Term Not Found"),"Sorry, the search term could not be found.");
            }
        }
}

/*Save Functionality - Saves to the same location as obtained from*/

void MainWindow::on_Save_clicked()
{
    if(chk_flag==1)
    {
        QString path;                                                                                                                  //Assigning a string path to store the absolute pathname
        path=ui->BrowsePath->text();                                                                                                   //taking the pathname from the string
        QFile file(path);                                                                                                              //opening the file specified
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))                                                                        //if it is unable to open the file
        return;                                                                                                                        //return
        QTextStream out(&file);                                                                                                        //write the file contents to the file
        QString str;
        str = ui->Input_Information->toPlainText();                                                                                    //converting inherent contents to be saved as a plaintext
        out<<str;                                                                                                                      //write the file
        file.close();
        ui->Alert->setText("File is Saved");
    }
    else
    {
        ui->Alert->setText("Invalid attempt");
    }

}

/*Sets Working Directory*/

void MainWindow::on_SetWorkingDirectory_clicked()
{
    baseDirectory1 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    baseDirectory = baseDirectory1+"\\Microsystem\\";
}

/*Selects the required software*/

void MainWindow::on_Selector_clicked()
{
    QString str;                                                                                                                        //Assign a string to store the value of selection
     if(ui->NISA->isChecked() == true)                                                                                                  //check if the radiobutton is selected
    {
    str = ui->NISA->text();                                                                                                             //if so then save the text of the radio button to the label
    ui->Select->setText(str);
    selection=1;
    nisa_filename=QFileInfo(source).fileName();
    }

    else if(ui->ABAQUS->isChecked() == true)
    {
    str = ui->ABAQUS->text();
    ui->Select->setText(str);
    selection=2;
    abaqus_filename=QFileInfo(source).fileName();
    }


    else if(ui->ANSYS->isChecked() == true)
    {
    str = ui->ANSYS->text();
    ui->Select->setText(str);
    selection=4;
    ansys_filename=QFileInfo(source).fileName();
    }



    else if(ui->HyFem->isChecked() == true)
    {
    str = ui->HyFem->text();
    ui->Select->setText(str);
    selection=6;
    hyfem_filename=QFileInfo(source).fileName();
    }


    else
    {
         selection = 0;
         ui->Alert->setText("Please select an option");
    }
}

void MainWindow::on_Transfer_clicked()
{

    if(ui->Select->text().isEmpty())
    {
        ui->Alert->setText("Please select first!");
    }
    else
    {

        ui->Alert->setText(" ");

        source=ui->BrowsePath->text();

        if(baseDirectory.isEmpty())
        {
            ui->Alert->setText("Please select Working Directory first!");
        }
        else
        {
        if(selection == 1)
        {
        QDir().mkdir(baseDirectory1 + "\\Microsystem");
        QDir().mkdir(baseDirectory + "\\NISA");

        QString destination=baseDirectory + "NISA\\"+QFileInfo(source).fileName();
        sourcer=destination;
        QFile::copy(source,destination);
        ui->Alert->setText("Transfer complete");

        }
        else if(selection == 2)
        {
            QDir().mkdir(baseDirectory1 + "\\Microsystem");
            QDir().mkdir(baseDirectory + "ABAQUS");
            QString destination=baseDirectory + "ABAQUS\\"+QFileInfo(source).fileName();
            QFile::copy(source,destination);
            ui->Alert->setText("Transformation complete");
        }

        else if(selection == 4)
        {
            QDir().mkdir(baseDirectory1 + "\\Microsystem");

            QDir().mkdir(baseDirectory + "ANSYS");
            QString destination=baseDirectory + "ANSYS\\"+QFileInfo(source).fileName();
            QFile::copy(source,destination);
            ui->Alert->setText("Transformation complete");
        }

        else if(selection == 6)
        {
            QDir().mkdir(baseDirectory1 + "\\Microsystem");

            QDir().mkdir(baseDirectory + "HyFem");
            QString destination=baseDirectory + "HyFem\\"+QFileInfo(source).fileName();
            QFile::copy(source,destination);
            ui->Alert->setText("Transformation complete");
        }

        }

    }

}
/*Data Parser */

//To enter User-Input
//Static input
void MainWindow::on_struct_static_ok_clicked()
{

    if(selection!=0)
    {
        if(ui->struct_stat_ls->text()!=NULL||ui->struct_stat_permitivity->text()!=NULL||ui->struct_stat_eleccon->text()!=NULL||ui->struct_stat_matid->text()!=NULL)
        {

               if (rx.exactMatch(ui->struct_stat_ls->text())&&rx.exactMatch(ui->struct_stat_permitivity->text())&&rx.exactMatch(ui->struct_stat_eleccon->text()))
               {
                    QString struct_stat_ls ="\n"+ ui->struct_stat_ls->text()+"\n";
                    QString struct_stat_matid=ui->struct_stat_matid->text();
                    QString struct_stat_perm="\n"+ui->struct_stat_permitivity->text()+"\n";
                    QString struct_stat_elecon="\n"+ui->struct_stat_eleccon->text()+"\n";
                    QString struct_stat_lsstore="*Load Step"+struct_stat_ls;
                    QString struct_data="*Material ID \n "+struct_stat_matid+"\n*Permitivity"+struct_stat_perm+"*Electrical Conductivity"+struct_stat_elecon;

                    QString path_nam=baseDirectory + "NISA\\Structural\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "NISA\\Structural\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);
                    if(selection == 1)
                    {

                        QDir().mkdir(baseDirectory + "NISA\\Structural");
                        QDir().mkdir(baseDirectory + "NISA\\Structural\\Static");
                        QString destiner_struct_nisa =baseDirectory+"NISA\\Structural\\Static\\";

                        QString stat_fname=baseDirectory + "NISA\\Structural\\Static\\DATA"+struct_stat_matid+".dat";
                        QFile file(stat_fname);                                       //opening the file specified
                        QFile filep(path_name+"\\DATA_PATH.dat");
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();
                        QString stat_lname=baseDirectory + "NISA\\Structural\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_stat_lsstore;                                                       //write the file
                        filel.close();

                         QFile::copy(sourcer,destiner_struct_nisa+QFileInfo(source).fileName());



                        if (!filep.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outp(&filep);                                         //write the file contents to the file
                        outp<<path_name;                                                       //write the file
                        filep.close();

                        QFile filenisa(storer+"\\parser\\Pathname.dat");
                        if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                        outnisa<<path_name;                                                       //write the file
                        filenisa.close();

                        QFile filenis(storer+"\\parser\\Filename.dat");
                        if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnis(&filenis);                                         //write the file contents to the file
                        outnis<<fil_name;                                                       //write the file
                        filenis.close();
                         ui->Alert_Parser->setText("Saved");

                    }
                    else if(selection == 2)
                    {

                        QDir().mkdir(baseDirectory + "ABAQUS\\Structural");
                        QDir().mkdir(baseDirectory + "ABAQUS\\Structural\\Static");
                         QString destiner_struct_abaqus =baseDirectory+"ANSYS\\Structural\\Static\\";
                        QString stat_fname=baseDirectory + "ABAQUS\\Structural\\Static\\DATA"+struct_stat_matid+".dat";

                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "ABAQUS\\Structural\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_stat_lsstore;                                                       //write the file
                        filel.close();
                        QFile::copy(sourcer,destiner_struct_abaqus+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ABAQUS\\Structural\\Static";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ABAQUS\\Structural\\Static\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                       if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                       outabaqus<<path_name;                                                       //write the file
                       fileabaqus.close();

                       QFile fileaba(storer+"\\parser\\Filename.dat");
                       if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outaba(&fileaba);                                         //write the file contents to the file
                       outaba<<fil_name;                                                       //write the file
                       fileaba.close();
                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 4)
                    {

                        QDir().mkdir(baseDirectory + "ANSYS\\Structural");
                        QDir().mkdir(baseDirectory + "ANSYS\\Structural\\Static");
                        QString destiner_struct_ansys =baseDirectory+"ANSYS\\Structural\\Static\\";
                        QString stat_fname=baseDirectory + "ANSYS\\Structural\\Static\\DATA"+struct_stat_matid+".dat";
                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "ANSYS\\Structural\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_stat_lsstore;                                                       //write the file
                        filel.close();
                        QFile::copy(sourcer,destiner_struct_ansys+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ANSYS\\Structural\\Static";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ANSYS\\Structural\\Static\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileansys(storer+"\\parser\\Pathname.dat");
                       if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outansys(&fileansys);                                         //write the file contents to the file
                       outansys<<path_name;                                                       //write the file
                       fileansys.close();

                       QFile fileans(storer+"\\parser\\Filename.dat");
                       if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outans(&fileans);                                         //write the file contents to the file
                       outans<<fil_name;                                                       //write the file
                       fileans.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 6)
                    {

                        QDir().mkdir(baseDirectory + "HyFem\\Structural");
                        QDir().mkdir(baseDirectory + "HyFem\\Structural\\Static");
                        QString destiner_struct_hyfem =baseDirectory+"HyFem\\Structural\\Static\\";
                        QString stat_fname=baseDirectory + "HyFem\\Structural\\Static\\DATA"+struct_stat_matid+".dat";

                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "HyFem\\Structural\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_stat_lsstore;                                                       //write the file
                        filel.close();
                         QFile::copy(sourcer,destiner_struct_hyfem+QFileInfo(source).fileName());

                         QString path_nam=baseDirectory + "HyFem\\Structural\\Static";
                         QString path_name=QDir::toNativeSeparators(path_nam);
                         QString file_name=baseDirectory + "HyFem\\Structural\\Static\\"+QFileInfo(source).fileName();
                         QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile filehyfem(storer+"\\parser\\Pathname.dat");
                        if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                        outhyfem<<path_name;                                                       //write the file
                        filehyfem.close();

                        QFile filehyf(storer+"\\parser\\Filename.dat");
                        if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                        outhyf<<fil_name;                                                       //write the file
                        filehyf.close();

                        ui->Alert_Parser->setText("Saved");
                    }

               }
               else
               {
                   ui->Alert_Parser->setText("Invalid Expression");
               }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }
}

void MainWindow::on_es_static_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->es_stat_ls->text()!=NULL&&ui->es_stat_permitivity->text()!=NULL&&ui->es_stat_eleccon->text()!=NULL&&ui->es_stat_matid->text()!=NULL)
        {
            if (rx.exactMatch(ui->es_stat_ls->text())&&rx.exactMatch(ui->es_stat_permitivity->text())&&rx.exactMatch(ui->es_stat_eleccon->text()))
            {
                QString e_stat_ls ="\n"+ ui->es_stat_ls->text()+"\n";
                QString e_stat_matid=ui->es_stat_matid->text();
                QString e_stat_perm="\n"+ui->es_stat_permitivity->text()+"\n";
                QString e_stat_elecon="\n"+ui->es_stat_eleccon->text()+"\n";
                QString e_stat_lsstore="*Load Step"+e_stat_ls;
                QString e_data="*Material ID\n"+e_stat_matid+"\n\n*Permitivity"+e_stat_perm+"*Electrical Conductivity"+e_stat_elecon;
                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Electrostatic");
                    QDir().mkdir(baseDirectory + "NISA\\Electrostatic\\Static");
                    QString destiner_electro_nisa =baseDirectory+"NISA\\Electrostatic\\Static\\";
                    QString stat_fname=baseDirectory + "NISA\\Electrostatic\\Static\\DATA"+e_stat_matid+".dat";
                    QString path_nam=baseDirectory + "NISA\\Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "NISA\\Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);

                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<e_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "NISA\\Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<e_stat_lsstore;                                                       //write the file
                    filel.close();
                    QFile::copy(sourcer,destiner_electro_nisa+QFileInfo(source).fileName());

                    QFile filenisa(storer+"\\parser\\Pathname.dat");
                    if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                    outnisa<<path_name;                                                       //write the file
                    filenisa.close();

                    QFile filenis(storer+"\\parser\\Filename.dat");
                    if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnis(&filenis);                                         //write the file contents to the file
                    outnis<<fil_name;                                                       //write the file
                    filenis.close();


                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Electrostatic");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Electrostatic\\Static");
                    QString destiner_electro_abaqus =baseDirectory+"ANSYS\\Electrostatic\\Static\\";
                    QString stat_fname=baseDirectory + "ABAQUS\\Electrostatic\\Static\\DATA"+e_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<e_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "ABAQUS\\Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<e_stat_lsstore;                                                       //write the file
                    filel.close();
                     QFile::copy(sourcer,destiner_electro_abaqus+QFileInfo(source).fileName());

                     QString path_nam=baseDirectory + "ABAQUS\\Electrostatic\\Static";
                     QString path_name=QDir::toNativeSeparators(path_nam);
                     QString file_name=baseDirectory + "ABAQUS\\Electrostatic\\Static\\"+QFileInfo(source).fileName();
                     QString fil_name=QDir::toNativeSeparators(file_name);


                    QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                    if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                    outabaqus<<path_name;                                                       //write the file
                    fileabaqus.close();

                    QFile fileaba(storer+"\\parser\\Filename.dat");
                    if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outaba(&fileaba);                                         //write the file contents to the file
                    outaba<<fil_name;                                                       //write the file
                    fileaba.close();
                     ui->Alert_Parser->setText("Saved");

                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Electrostatic");
                    QDir().mkdir(baseDirectory + "ANSYS\\Electrostatic\\Static");
                     QString destiner_electro_ansys =baseDirectory+"ANSYS\\Electrostatic\\Static\\";
                    QString stat_fname=baseDirectory + "ANSYS\\Electrostatic\\Static\\DATA"+e_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<e_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "ANSYS\\Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<e_stat_lsstore;                                                       //write the file
                    filel.close();
                    QFile::copy(sourcer,destiner_electro_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;                                                       //write the file
                   fileans.close();
                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Electrostatic");
                    QDir().mkdir(baseDirectory + "HyFem\\Electrostatic\\Static");
                    QString destiner_electro_hyfem =baseDirectory+"HyFem\\Electrostatic\\Static\\";
                    QString stat_fname=baseDirectory + "HyFem\\Electrostatic\\Static\\DATA"+e_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<e_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "HyFem\\Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<e_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_electro_hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();

                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }

        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }

}


void MainWindow::on_cse_static_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->cse_stat_ls->text()!=NULL&&ui->cse_stat_permitivity->text()!=NULL&&ui->cse_stat_eleccon->text()!=NULL&&ui->cse_stat_matid->text()!=NULL)
        {
            if (rx.exactMatch(ui->cse_stat_ls->text())&&rx.exactMatch(ui->cse_stat_permitivity->text())&&rx.exactMatch(ui->cse_stat_eleccon->text()))
            {
                QString cse_stat_ls = "\n"+ui->cse_stat_ls->text()+"\n";
                QString cse_stat_matid=ui->cse_stat_matid->text();
                QString cse_stat_perm="\n"+ui->cse_stat_permitivity->text()+"\n";
                QString cse_stat_elecon="\n"+ui->cse_stat_eleccon->text()+"\n";
                QString cse_stat_lsstore="*Load Step"+cse_stat_ls;
                QString cse_data="*Material ID\n"+cse_stat_matid+"\n\n*Permitivity"+cse_stat_perm+"*Electrical Conductivity"+cse_stat_elecon;


                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Coupled_Structure_Electrostatic");
                    QString destiner_cse_nisa =baseDirectory+"NISA\\Coupled_Structure_Electrostatic\\Static\\";
                    QDir().mkdir(baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Static");
                    QString stat_fname=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Static\\DATA"+cse_stat_matid+".dat";


                    QString path_nam=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<cse_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<cse_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_cse_nisa+QFileInfo(source).fileName());



                    QFile filenisa(storer+"\\parser\\Pathname.dat");
                    if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                    outnisa<<path_name;                                                       //write the file
                    filenisa.close();

                    QFile filenis(storer+"\\parser\\Filename.dat");
                    if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnis(&filenis);                                         //write the file contents to the file
                    outnis<<fil_name;                                                       //write the file
                    filenis.close();

                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Static");
                    QString destiner_cse_abaqus =baseDirectory+"ABAQUS\\Coupled_Structure_Electrostatic\\Static\\";
                    QString stat_fname=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Static\\DATA"+cse_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<cse_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<cse_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_cse_abaqus+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                   if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                   outabaqus<<path_name;                                                       //write the file
                   fileabaqus.close();

                   QFile fileaba(storer+"\\parser\\Filename.dat");
                   if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outaba(&fileaba);                                         //write the file contents to the file
                   outaba<<fil_name;                                                       //write the file
                   fileaba.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic");
                    QDir().mkdir(baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Static");
                    QString destiner_cse_ansys =baseDirectory+"ANSYS\\Coupled_Structure_Electrostatic\\Static\\";
                     QString stat_fname=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Static\\DATA"+cse_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<cse_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<cse_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_cse_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;                                                       //write the file
                   fileans.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Coupled_Structure_Electrostatic");
                    QDir().mkdir(baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Static");
                    QString destiner_cse_Hyfem =baseDirectory+"HyFem\\Coupled_Structure_Electrostatic\\Static\\";
                     QString stat_fname=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Static\\DATA"+cse_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<cse_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<cse_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_cse_Hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();

                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
}
else
{
    ui->Alert_Parser->setText("Please complete transform first");
}
}

void MainWindow::on_pe_static_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->pe_stat_ls->text()!=NULL&&ui->pe_stat_permitivity->text()!=NULL&&ui->pe_stat_eleccon->text()!=NULL&&ui->pr_stat_matid->text()!=NULL)
        {
            if (rx.exactMatch(ui->pe_stat_ls->text())&&rx.exactMatch(ui->pe_stat_permitivity->text())&&rx.exactMatch(ui->pe_stat_eleccon->text()))
            {
                QString pe_stat_ls ="\n"+ ui->pe_stat_ls->text()+"\n";
                QString pe_stat_matid=ui->pr_stat_matid->text();
                QString pe_stat_perm="\n"+ui->pe_stat_permitivity->text()+"\n";
                QString pe_stat_elecon="\n"+ui->pe_stat_eleccon->text()+"\n";
                QString pe_stat_lsstore="*Load Step"+pe_stat_ls;
                QString pe_data="*Material ID\n"+pe_stat_matid+"\n\n*Permitivity"+pe_stat_perm+"*Electrical Conductivity"+pe_stat_elecon;
                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Piezo_Electricity");
                    QDir().mkdir(baseDirectory + "NISA\\Piezo_Electricity\\Static");
                    QString destiner_pe_nisa =baseDirectory+"NISA\\Piezo_Electricity\\Static\\";

                    QString stat_fname=baseDirectory + "NISA\\Piezo_Electricity\\Static\\DATA"+pe_stat_matid+".dat";


                    QString path_nam=baseDirectory + "NISA\\Piezo_Electricity\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "NISA\\Piezo_Electricity\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pe_data;                                                       //write the file
                    file.close();

                    QString stat_lname=baseDirectory + "NISA\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pe_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_pe_nisa+QFileInfo(source).fileName());




                    QFile filenisa(storer+"\\parser\\Pathname.dat");
                    if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                    outnisa<<path_name;                                                       //write the file
                    filenisa.close();

                    QFile filenis(storer+"\\parser\\Filename.dat");
                    if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnis(&filenis);                                         //write the file contents to the file
                    outnis<<fil_name;                                                       //write the file
                    filenis.close();

                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Electricity");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Electricity\\Static");
                    QString destiner_pe_abaqus =baseDirectory+"ABAQUS\\Piezo_Electricity\\Static\\";

                    QString stat_fname=baseDirectory + "ABAQUS\\Piezo_Electricity\\Static\\DATA"+pe_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pe_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "NISA\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pe_stat_lsstore;                                                       //write the file
                    filel.close();

                     QFile::copy(sourcer,destiner_pe_abaqus+QFileInfo(source).fileName());

                     QString path_nam=baseDirectory + "ABAQUS\\Piezo_Electricity\\Static";
                     QString path_name=QDir::toNativeSeparators(path_nam);
                     QString file_name=baseDirectory + "ABAQUS\\Piezo_Electricity\\Static\\"+QFileInfo(source).fileName();
                     QString fil_name=QDir::toNativeSeparators(file_name);


                    QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                    if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                    outabaqus<<path_name;                                                       //write the file
                    fileabaqus.close();

                    QFile fileaba(storer+"\\parser\\Filename.dat");
                    if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outaba(&fileaba);                                         //write the file contents to the file
                    outaba<<fil_name;                                                       //write the file
                    fileaba.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Electricity");
                    QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Electricity\\Static");
                     QString destiner_pe_ansys=baseDirectory+"ANSYS\\Piezo_Electricity\\Static\\";



                    QString stat_fname=baseDirectory + "ANSYS\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pe_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "ANSYS\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pe_stat_lsstore;                                                       //write the file
                    filel.close();
                    QFile::copy(sourcer,destiner_pe_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Piezo_Electicity\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Piezo_Electricity\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;                                                       //write the file
                   fileans.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Piezo_Electricity");
                    QDir().mkdir(baseDirectory + "HyFem\\Piezo_Electricity\\Static");
                    QString destiner_pe_hyfem=baseDirectory+"ANSYS\\Piezo_Electricity\\Static\\";
                    QString stat_fname=baseDirectory + "HyFem\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pe_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "HyFem\\Piezo_Electricity\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pe_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_pe_hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Piezo_Electricity\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Piezo_Electricity\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();

                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invaalid Entry");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }
}

void MainWindow::on_pr_static_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->pr_stat_ls->text()!=NULL&&ui->pr_stat_permitivity->text()!=NULL&&ui->pr_stat_eleccon->text()!=NULL&&ui->pe_stat_matid->text()!=NULL)
        {
            if (rx.exactMatch(ui->pr_stat_ls->text())&&rx.exactMatch(ui->pr_stat_permitivity->text())&&rx.exactMatch(ui->pr_stat_eleccon->text()))
            {
                 QString pr_stat_ls ="\n"+ ui->pr_stat_ls->text()+"\n";
                 QString pr_stat_matid=ui->pe_stat_matid->text();
                 QString pr_stat_perm="\n"+ui->pr_stat_permitivity->text()+"\n";
                 QString pr_stat_elecon="\n"+ui->pr_stat_eleccon->text()+"\n";
                 QString pr_stat_lsstore="*Load Step"+pr_stat_ls;
                    QString pr_data="*Material ID\n"+pr_stat_matid+"\n\n*Permitivity"+pr_stat_perm+"*Electrical Conductivity"+pr_stat_elecon;
                    if(selection == 1)
                    {

                        QDir().mkdir(baseDirectory + "NISA\\Piezo_Resistivity");
                         QDir().mkdir(baseDirectory + "NISA\\Piezo_Resistivity\\Static");
                          QString destiner_pr_nisa=baseDirectory+"NISA\\Piezo_Resistivity\\Static\\";
                         QString stat_fname=baseDirectory + "NISA\\Piezo_Resistivity\\Static\\DATA"+pr_stat_matid+".dat";

                         QString path_nam=baseDirectory + "NISA\\Piezo_Resistivity\\Static";
                         QString path_name=QDir::toNativeSeparators(path_nam);
                         QString file_name=baseDirectory + "NISA\\Piezo_Resistivity\\Static\\"+QFileInfo(source).fileName();
                         QString fil_name=QDir::toNativeSeparators(file_name);

                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pr_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "NISA\\Piezo_Resistivity\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pr_stat_lsstore;                                                       //write the file
                        filel.close();
                         QFile::copy(sourcer,destiner_pr_nisa+QFileInfo(source).fileName());




                          QFile filenisa(storer+"\\parser\\Pathname.dat");
                         if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                         return;                                                         //return
                         QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                         outnisa<<path_name;                                                       //write the file
                         filenisa.close();

                         QFile filenis(storer+"\\parser\\Filename.dat");
                         if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                         return;                                                         //return
                         QTextStream outnis(&filenis);                                         //write the file contents to the file
                         outnis<<fil_name;                                                       //write the file
                         filenis.close();

                        ui->Alert_Parser->setText("Saved");
                    }
                    else if(selection == 2)
                    {

                        QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Resistivity");
                        QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Resistivity\\Static");
                        QString destiner_pr_abaqus=baseDirectory+"ABAQUS\\Piezo_Resistivity\\Static\\";
                        QString stat_fname=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Static\\DATA"+pr_stat_matid+".dat";
                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pr_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pr_stat_lsstore;                                                       //write the file
                        filel.close();
                        QFile::copy(sourcer,destiner_pr_abaqus+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Static";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Static\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                       if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                       outabaqus<<path_name;                                                       //write the file
                       fileabaqus.close();

                       QFile fileaba(storer+"\\parser\\Filename.dat");
                       if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outaba(&fileaba);                                         //write the file contents to the file
                       outaba<<fil_name;                                                       //write the file
                       fileaba.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 4)
                    {

                        QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Resistivity");
                        QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Resistivity\\Static");
                        QString destiner_pr_ansys=baseDirectory+"ANSYS\\Piezo_Resistivity\\Static\\";
                        QString stat_fname=baseDirectory + "ANSYS\\Piezo_Resistivity\\Static\\DATA"+pr_stat_matid+".dat";
                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pr_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "ANSYS\\Piezo_Resistivity\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pr_stat_lsstore;                                                       //write the file
                        filel.close();

                        QFile::copy(sourcer,destiner_pr_ansys+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ANSYS\\Piezo_Resistivity\\Static";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ANSYS\\Piezo_Resistivity\\Static\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileansys(storer+"\\parser\\Pathname.dat");
                       if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outansys(&fileansys);                                         //write the file contents to the file
                       outansys<<path_name;                                                       //write the file
                       fileansys.close();

                       QFile fileans(storer+"\\parser\\Filename.dat");
                       if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outans(&fileans);                                         //write the file contents to the file
                       outans<<fil_name;                                                       //write the file
                       fileans.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 6)
                    {

                        QDir().mkdir(baseDirectory + "HyFem\\Piezo_Resistivity");
                        QDir().mkdir(baseDirectory + "HyFem\\Piezo_Resistivity\\Static");
                        QString destiner_pr_hyfem=baseDirectory+"HyFem\\Piezo_Resistivity\\Static\\";
                        QString stat_fname=baseDirectory + "HyFem\\Piezo_Resistivity\\Static\\DATA"+pr_stat_matid+".dat";
                        QFile file(stat_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pr_data;                                                       //write the file
                        file.close();

                        QString stat_lname=baseDirectory + "HyFem\\Piezo_Resistivity\\Static\\DATA.dat";
                        QFile filel(stat_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pr_stat_lsstore;                                                       //write the file
                        filel.close();

                        QFile::copy(sourcer,destiner_pr_hyfem+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "HyFem\\Piezo_Resistivity\\Static";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "HyFem\\Piezo_Resistivity\\Static\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile filehyfem(storer+"\\parser\\Pathname.dat");
                       if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                       outhyfem<<path_name;                                                       //write the file
                       filehyfem.close();

                       QFile filehyf(storer+"\\parser\\Filename.dat");
                       if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                       outhyf<<fil_name;                                                       //write the file
                       filehyf.close();

                        ui->Alert_Parser->setText("Saved");
                    }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid entry");
            }
            }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }

}

void MainWindow::on_sf_static_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->sf_stat_ls->text()!=NULL&&ui->sf_stat_permitivity->text()!=NULL&&ui->sf_stat_eleccon->text()!=NULL&&ui->sf_stat_matid->text()!=NULL)
        {
            if (rx.exactMatch(ui->sf_stat_ls->text())&&rx.exactMatch(ui->sf_stat_permitivity->text())&&rx.exactMatch(ui->sf_stat_eleccon->text()))
            {

                QString sf_stat_ls ="\n"+ ui->sf_stat_ls->text()+"\n";
                QString sf_stat_matid=ui->sf_stat_matid->text();
                QString sf_stat_perm="\n"+ui->sf_stat_permitivity->text()+"\n";
                QString sf_stat_elecon="\n"+ui->sf_stat_eleccon->text()+"\n";
                QString sf_stat_lsstore="*Load Step"+sf_stat_ls;
                 QString sf_data="*Material ID\n"+sf_stat_matid+"\n*Permitivity"+sf_stat_perm+"*Electrical Conductivity"+sf_stat_elecon;
                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "NISA\\Squeeze_Film\\Static");
                    QString destiner_sf_nisa=baseDirectory+"NISA\\Squeeze_Film\\Static\\";
                    QString stat_fname=baseDirectory + "NISA\\Squeeze_Film\\Static\\DATA"+sf_stat_matid+".dat";

                    QString path_nam=baseDirectory + "NISA\\Squeeze_Film\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "NISA\\Squeeze_Film\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);

                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "NISA\\Squeeze_Film\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_stat_lsstore;                                                       //write the file
                    filel.close();

                    QFile::copy(sourcer,destiner_sf_nisa+QFileInfo(source).fileName());



                    QFile filenisa(storer+"\\parser\\Pathname.dat");
                    if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                    outnisa<<path_name;                                                       //write the file
                    filenisa.close();

                    QFile filenis(storer+"\\parser\\Filename.dat");
                    if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outnis(&filenis);                                         //write the file contents to the file
                    outnis<<fil_name;                                                       //write the file
                    filenis.close();
                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Squeeze_Film\\Static");

                    QString destiner_sf_abaqus=baseDirectory+"ABAQUS\\Squeeze_Film\\Static\\";
                    QString stat_fname=baseDirectory + "ABAQUS\\Squeeze_Film\\Static\\DATA"+sf_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "ABAQUS\\Squeeze_Film\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_stat_lsstore;                                                       //write the file
                    filel.close();

                     QFile::copy(sourcer,destiner_sf_abaqus+QFileInfo(source).fileName());

                     QString path_nam=baseDirectory + "ABAQUS\\Squeeze_Film\\Static";
                     QString path_name=QDir::toNativeSeparators(path_nam);
                     QString file_name=baseDirectory + "ABAQUS\\Squeeze_Film\\Static\\"+QFileInfo(source).fileName();
                     QString fil_name=QDir::toNativeSeparators(file_name);


                    QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                    if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                    outabaqus<<path_name;                                                       //write the file
                    fileabaqus.close();

                    QFile fileaba(storer+"\\parser\\Filename.dat");
                    if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outaba(&fileaba);                                         //write the file contents to the file
                    outaba<<fil_name;                                                       //write the file
                    fileaba.close();
                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "ANSYS\\Squeeze_Film\\Static");
                    QString destiner_sf_ansys=baseDirectory+"ANSYS\\Squeeze_Film\\Static\\";
                    QString stat_fname=baseDirectory + "ANSYS\\Squeeze_Film\\Static\\DATA"+sf_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "ANSYS\\Squeeze_Film\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_stat_lsstore;                                                       //write the file
                    filel.close();
                    QFile::copy(sourcer,destiner_sf_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Squeeze_Film\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Squeeze_Film\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;                                                       //write the file
                   fileans.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "HyFem\\Squeeze_Film\\Static");
                     QString destiner_sf_hyfem=baseDirectory+"HyFem\\Squeeze_Film\\Static\\";
                    QString stat_fname=baseDirectory + "HyFem\\Squeeze_Film\\Static\\DATA"+sf_stat_matid+".dat";
                    QFile file(stat_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();
                    QString stat_lname=baseDirectory + "HyFem\\Squeeze_Film\\Static\\DATA.dat";
                    QFile filel(stat_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_stat_lsstore;                                                       //write the file
                    filel.close();
                    QFile::copy(sourcer,destiner_sf_hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Squeeze_Film\\Static";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Squeeze_Film\\Static\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();

                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }
}
//static ok ends here


//static reset

void MainWindow::on_struct_static_reset_clicked()
{
    ui->struct_stat_ls->setText("0");
    ui->struct_stat_matid->setValue(1);
    ui->struct_stat_permitivity->setText("0");
    ui->struct_stat_eleccon->setText("0");
    ui->Alert_Parser->setText("Reset Complete");

}

void MainWindow::on_es_static_reset_clicked()
{
    ui->es_stat_ls->setText("0");
    ui->es_stat_matid->setValue(1);
    ui->es_stat_permitivity->setText("0");
    ui->es_stat_eleccon->setText("0");
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_cse_static_reset_clicked()
{
    ui->cse_stat_ls->setText("0");
    ui->cse_stat_permitivity->setText("0");
    ui->cse_stat_eleccon->setText("0");
    ui->cse_stat_matid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_pe_static_reset_clicked()
{
    ui->pe_stat_ls->setText("0");
    ui->pe_stat_permitivity->setText("0");
    ui->pe_stat_eleccon->setText("0");
    ui->pe_stat_matid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_pr_static_reset_clicked()
{
    ui->pr_stat_ls->setText("0");
    ui->pr_stat_permitivity->setText("0");
    ui->pr_stat_eleccon->setText("0");
    ui->pr_stat_matid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_sf_static_reset_clicked()
{
    ui->sf_stat_ls->setText("0");
    ui->sf_stat_matid->setValue(1);
    ui->sf_stat_permitivity->setText("0");
    ui->sf_stat_eleccon->setText("0");
    ui->Alert_Parser->setText("Reset Complete");
}
//static rest ends here



//transient data input
void MainWindow::on_modal_trans_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->sf_trans_ls->text()!=NULL&&ui->sf_trans_perm->text()!=NULL&&ui->sf_trans_eleccon->text()!=NULL&&ui->sf_trans_strt->text()!=NULL&&ui->sf_trans_nd->text()!=NULL&&ui->sf_trans_increment->text()!=NULL&&ui->sf_trans_materialid->text()!=NULL)
        {
            if (rx.exactMatch(ui->sf_trans_ls->text())&&rx.exactMatch(ui->sf_trans_perm->text())&&rx.exactMatch(ui->sf_trans_eleccon->text())&&rx.exactMatch(ui->sf_trans_strt->text())&&rx.exactMatch(ui->sf_trans_nd->text())&&rx.exactMatch(ui->sf_trans_increment->text()))
            {
                QString sf_trans_ls ="\n"+ ui->sf_trans_ls->text()+"\n";
                QString sf_trans_matid=ui->sf_trans_materialid->text();
                QString sf_trans_perm="\n"+ui->sf_trans_perm->text()+"\n";
                QString sf_trans_elecon="\n"+ui->sf_trans_eleccon->text()+"\n";
                QString sf_trans_timestr="\n"+ui->sf_trans_strt->text()+"\n";
                QString sf_trans_timeend="\n"+ui->sf_trans_nd->text()+"\n";
                QString sf_trans_timeinc="\n"+ui->sf_trans_increment->text()+"\n";
                QString sf_trans_ldata="*Load Step"+sf_trans_ls;
                QString sf_trans_tdata="*Time Start:"+sf_trans_timestr+"*Time End:"+sf_trans_timeend+"*Time Increment:"+sf_trans_timeinc;
                QString sf_data="*Material ID"+sf_trans_matid+"\n*Permitivity"+sf_trans_perm+"*Electrical Conductivity"+sf_trans_elecon;
                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "NISA\\Squeeze_Film\\Transient");
                     QString destine_sf_nisa=baseDirectory+"NISA\\Squeeze_Film\\Transient\\";
                    QString trans_fname=baseDirectory + "NISA\\Squeeze_Film\\Transient\\DATA"+sf_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "NISA\\Squeeze_Film\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "NISA\\Squeeze_Film\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<sf_trans_tdata;                                                       //write the file
                    filet.close();

                     QFile::copy(sourcer,destine_sf_nisa+QFileInfo(source).fileName());

                     QString path_nam=baseDirectory + "NISA\\Squeeze_Film\\Transient";
                     QString path_name=QDir::toNativeSeparators(path_nam);
                     QString file_name=baseDirectory + "NISA\\Squeeze_Film\\Transient\\"+QFileInfo(source).fileName();
                     QString fil_name=QDir::toNativeSeparators(file_name);


                     QFile filenisa(storer+"\\parser\\Pathname.dat");
                     if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                     return;                                                         //return
                     QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                     outnisa<<path_name;                                                       //write the file
                     filenisa.close();

                     QFile filenis(storer+"\\parser\\Filename.dat");
                     if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                     return;                                                         //return
                     QTextStream outnis(&filenis);                                         //write the file contents to the file
                     outnis<<fil_name;                                                       //write the file
                     filenis.close();

                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Squeeze_Film\\Transient");
                    QString destine_sf_abaqus=baseDirectory+"ABAQUS\\Squeeze_Film\\Transient\\";
                    QString trans_fname=baseDirectory + "ABAQUS\\Squeeze_Film\\Transient\\DATA"+sf_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "ABAQUS\\Squeeze_Film\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "ABAQUS\\Squeeze_Film\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<sf_trans_tdata;                                                       //write the file
                    filet.close();
                    QFile::copy(sourcer,destine_sf_abaqus+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ABAQUS\\Squeeze_Film\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ABAQUS\\Squeeze_Film\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                   if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                   outabaqus<<path_name;                                                       //write the file
                   fileabaqus.close();

                   QFile fileaba(storer+"\\parser\\Filename.dat");
                   if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outaba(&fileaba);                                         //write the file contents to the file
                   outaba<<fil_name;                                                       //write the file
                   fileaba.close();

                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "ANSYS\\Squeeze_Film\\Transient");
                    QString destine_sf_ansys=baseDirectory+"ANSYS\\Squeeze_Film\\Transient\\";
                    QString trans_fname=baseDirectory + "ANSYS\\Squeeze_Film\\Transient\\DATA"+sf_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "ANSYS\\Squeeze_Film\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "ANSYS\\Squeeze_Film\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<sf_trans_tdata;                                                       //write the file
                    filet.close();
                    QFile::copy(sourcer,destine_sf_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Squeeze_Film\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Squeeze_Film\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;


                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Squeeze_Film");
                    QDir().mkdir(baseDirectory + "HyFem\\Squeeze_Film\\Transient");
                    QString destine_sf_hyfem=baseDirectory+"HyFem\\Squeeze_Film\\Transient\\";
                    QString trans_fname=baseDirectory + "HyFem\\Squeeze_Film\\Transient\\DATA"+sf_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<sf_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "HyFem\\Squeeze_Film\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<sf_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "HyFem\\Squeeze_Film\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<sf_trans_tdata;                                                       //write the file
                    filet.close();

                    QFile::copy(sourcer,destine_sf_hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Squeeze_Film\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Squeeze_Film\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();

                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }

}

void MainWindow::on_struct_trans_ok_clicked()
{
        if(selection!=0)
        {
            if(ui->struct_trans_ls->text()!=NULL&&ui->struct_trans_perm->text()!=NULL&&ui->struct_trans_eleccon->text()!=NULL&&ui->struct_trans_strt->text()!=NULL&&ui->struct_trans_nd->text()!=NULL&&ui->struct_trans_increment->text()!=NULL&&ui->struct_trans_materialid->text()!=NULL)
            {
                if (rx.exactMatch(ui->struct_trans_ls->text())&&rx.exactMatch(ui->struct_trans_perm->text())&&rx.exactMatch(ui->struct_trans_eleccon->text())&&rx.exactMatch(ui->struct_trans_strt->text())&&rx.exactMatch(ui->struct_trans_nd->text())&&rx.exactMatch(ui->struct_trans_increment->text()))
                {
                     QString struct_trans_ls ="\n"+ ui->struct_trans_ls->text()+"\n";
                    QString struct_trans_matid=ui->struct_trans_materialid->text();
                    QString struct_trans_perm="\n"+ui->struct_trans_perm->text()+"\n";
                    QString struct_trans_elecon="\n"+ui->struct_trans_eleccon->text()+"\n";
                    QString struct_trans_timestr="\n"+ui->struct_trans_strt->text()+"\n";
                    QString struct_trans_timeend="\n"+ui->struct_trans_nd->text()+"\n";
                    QString struct_trans_timeinc="\n"+ui->struct_trans_increment->text()+"\n";
                    QString struct_trans_ldata="*Load Step"+struct_trans_ls;
                    QString struct_trans_tdata="*Time Start:"+struct_trans_timestr+"*Time End:"+struct_trans_timeend+"*Time Increment:"+struct_trans_timeinc;
                    QString struct_data="*Material ID\n"+struct_trans_matid+"\n*Permitivity"+struct_trans_perm+"*Electrical Conductivity"+struct_trans_elecon;
                    if(selection == 1)
                    {

                        QDir().mkdir(baseDirectory + "NISA\\Structural");
                        QDir().mkdir(baseDirectory + "NISA\\Structural\\Transient");
                        QString destine_struct_nisa=baseDirectory+"NISA\\Structural\\Transient\\";

                        QString trans_fname=baseDirectory + "NISA\\Structural\\Transient\\DATA"+struct_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "NISA\\Structural\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "NISA\\Structural\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<struct_trans_tdata;                                                       //write the file
                        filet.close();

                        QFile::copy(sourcer,destine_struct_nisa+QFileInfo(source).fileName());


                        QString path_nam=baseDirectory + "NISA\\Structural\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "NISA\\Structural\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile filenisa(storer+"\\parser\\Pathname.dat");
                        if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                        outnisa<<path_name;                                                       //write the file
                        filenisa.close();

                        QFile filenis(storer+"\\parser\\Filename.dat");
                        if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnis(&filenis);                                         //write the file contents to the file
                        outnis<<fil_name;                                                       //write the file
                        filenis.close();


                        ui->Alert_Parser->setText("Saved");
                    }
                    else if(selection == 2)
                    {

                        QDir().mkdir(baseDirectory + "ABAQUS\\Structural");
                        QDir().mkdir(baseDirectory + "ABAQUS\\Structural\\Transient");
                        QString destine_struct_abaqus=baseDirectory+"ABAQUS\\Structural\\Transient\\";
                        QString trans_fname=baseDirectory + "ABAQUS\\Structural\\Transient\\DATA"+struct_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ABAQUS\\Structural\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ABAQUS\\Structural\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<struct_trans_tdata;                                                       //write the file
                        filet.close();
                         QFile::copy(sourcer,destine_struct_abaqus+QFileInfo(source).fileName());

                         QString path_nam=baseDirectory + "ABAQUS\\Structural\\Transient";
                         QString path_name=QDir::toNativeSeparators(path_nam);
                         QString file_name=baseDirectory + "ABAQUS\\Structural\\Transient\\"+QFileInfo(source).fileName();
                         QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                        if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                        outabaqus<<path_name;                                                       //write the file
                        fileabaqus.close();

                        QFile fileaba(storer+"\\parser\\Filename.dat");
                        if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outaba(&fileaba);                                         //write the file contents to the file
                        outaba<<fil_name;                                                       //write the file
                        fileaba.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 4)
                    {

                        QDir().mkdir(baseDirectory + "ANSYS\\Structural");
                        QDir().mkdir(baseDirectory + "ANSYS\\Structural\\Transient");
                        QString destine_struct_ansys=baseDirectory+"ANSYS\\Structural\\Transient\\";
                        QString trans_fname=baseDirectory + "ANSYS\\Structural\\Transient\\DATA"+struct_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ANSYS\\Structural\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ANSYS\\Structural\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<struct_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_struct_ansys+QFileInfo(source).fileName());


                        QString path_nam=baseDirectory + "ANSYS\\Structural\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ANSYS\\Structural\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileansys(storer+"\\parser\\Pathname.dat");
                       if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outansys(&fileansys);                                         //write the file contents to the file
                       outansys<<path_name;                                                       //write the file
                       fileansys.close();

                       QFile fileans(storer+"\\parser\\Filename.dat");
                       if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outans(&fileans);                                         //write the file contents to the file
                       outans<<fil_name;                                                       //write the file
                       fileans.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 6)
                    {

                        QDir().mkdir(baseDirectory + "HyFem\\Structural");
                        QDir().mkdir(baseDirectory + "HyFem\\Structural\\Transient");
                        QString destine_struct_HyFem=baseDirectory+"HyFem\\Structural\\Transient\\";
                        QString trans_fname=baseDirectory + "HyFem\\Structural\\Transient\\DATA"+struct_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<struct_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "HyFem\\Structural\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<struct_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "HyFem\\Structural\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<struct_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_struct_HyFem+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "HyFem\\Structural\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "HyFem\\Structural\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile filehyfem(storer+"\\parser\\Pathname.dat");
                       if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                       outhyfem<<path_name;                                                       //write the file
                       filehyfem.close();

                       QFile filehyf(storer+"\\parser\\Filename.dat");
                       if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                       outhyf<<fil_name;                                                       //write the file
                       filehyf.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                }
                else
                {
                    ui->Alert_Parser->setText("Invalid Text");
                }
            }
            else
            {
                ui->Alert_Parser->setText("Please Enter all the details");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please complete transform first");
        }

}

void MainWindow::on_pe_trans_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->pe_trans_ls->text()!=NULL&&ui->pe_trans_perm->text()!=NULL&&ui->pe_trans_eleccon->text()!=NULL&&ui->pe_trans_nd->text()!=NULL&&ui->pe_trans_strt->text()!=NULL&&ui->pe_trans_increment->text()!=NULL&&ui->pe_trans_materialid->text()!=NULL)
        {
            if (rx.exactMatch(ui->pe_trans_ls->text())&&rx.exactMatch(ui->pe_trans_perm->text())&&rx.exactMatch(ui->pe_trans_eleccon->text())&&rx.exactMatch(ui->pe_trans_nd->text())&&rx.exactMatch(ui->pe_trans_strt->text())&&rx.exactMatch(ui->pe_trans_increment->text()))
            {
                    QString pe_trans_ls ="\n"+ ui->pe_trans_ls->text()+"\n";
                    QString pe_trans_matid=ui->pe_trans_materialid->text();
                    QString pe_trans_perm="\n"+ui->pe_trans_perm->text()+"\n";
                    QString pe_trans_elecon="\n"+ui->pe_trans_eleccon->text()+"\n";
                    QString pe_trans_timestr="\n"+ui->pe_trans_strt->text()+"\n";
                    QString pe_trans_timeend="\n"+ui->pe_trans_nd->text()+"\n";
                    QString pe_trans_timeinc="\n"+ui->pe_trans_increment->text()+"\n";
                    QString pe_trans_ldata="*Load Step"+pe_trans_ls;
                    QString pe_trans_tdata="*Time Start:"+pe_trans_timestr+"*Time End:"+pe_trans_timeend+"*Time Increment:"+pe_trans_timeinc;
                    QString pe_data="*Material ID"+pe_trans_matid+"\n*Permitivity"+pe_trans_perm+"*Electrical Conductivity"+pe_trans_elecon;
                    if(selection == 1)
                    {

                        QDir().mkdir(baseDirectory + "NISA\\Piezo_Electricity");
                        QDir().mkdir(baseDirectory + "NISA\\Piezo_Electricity\\Transient");
                        QString destine_pe_nisa=baseDirectory+"NISA\\Piezo_Electricity\\Transient\\";
                        QString trans_fname=baseDirectory + "NISA\\Piezo_Electricity\\Transient\\DATA"+pe_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pe_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "NISA\\Piezo_Electricity\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pe_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "NISA\\Piezo_Electricity\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<pe_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_pe_nisa+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "NISA\\Piezo_Electricity\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "NISA\\Piezo_Electricity\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile filenisa(storer+"\\parser\\Pathname.dat");
                        if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                        outnisa<<path_name;                                                       //write the file
                        filenisa.close();

                        QFile filenis(storer+"\\parser\\Filename.dat");
                        if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnis(&filenis);                                         //write the file contents to the file
                        outnis<<fil_name;                                                       //write the file
                        filenis.close();

                       ui->Alert_Parser->setText("Saved");
                    }
                    else if(selection == 2)
                    {

                        QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Electricity");
                        QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient");
                        QString destine_pe_abaqus=baseDirectory+"ABAQUS\\Piezo_Electricity\\Transient\\";
                        QString trans_fname=baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient\\DATA"+pe_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pe_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pe_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<pe_trans_tdata;                                                       //write the file
                        filet.close();
                         QFile::copy(sourcer,destine_pe_abaqus+QFileInfo(source).fileName());

                         QString path_nam=baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient";
                         QString path_name=QDir::toNativeSeparators(path_nam);
                         QString file_name=baseDirectory + "ABAQUS\\Piezo_Electricity\\Transient\\"+QFileInfo(source).fileName();
                         QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                        if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                        outabaqus<<path_name;                                                       //write the file
                        fileabaqus.close();

                        QFile fileaba(storer+"\\parser\\Filename.dat");
                        if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outaba(&fileaba);                                         //write the file contents to the file
                        outaba<<fil_name;                                                       //write the file
                        fileaba.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 4)
                    {

                        QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Electricity");
                        QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Electricity\\Transient");
                        QString destine_pe_ansys=baseDirectory+"ANSYS\\Piezo_Electricity\\Transient\\";
                        QString trans_fname=baseDirectory + "ANSYS\\Piezo_Electricity\\Transient\\DATA"+pe_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pe_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ANSYS\\Piezo_Electricity\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pe_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ANSYS\\Piezo_Electricity\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<pe_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_pe_ansys+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ANSYS\\Piezo_Electricity\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ANSYS\\Piezo_Electricity\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileansys(storer+"\\parser\\Pathname.dat");
                       if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outansys(&fileansys);                                         //write the file contents to the file
                       outansys<<path_name;                                                       //write the file
                       fileansys.close();

                       QFile fileans(storer+"\\parser\\Filename.dat");
                       if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outans(&fileans);                                         //write the file contents to the file
                       outans<<fil_name;                                                       //write the file
                       fileans.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 6)
                    {

                        QDir().mkdir(baseDirectory + "HyFem\\Piezo_Electricity");
                        QDir().mkdir(baseDirectory + "HyFem\\Piezo_Electricity\\Transient");
                        QString destine_pe_hyfem=baseDirectory+"HyFem\\Piezo_Electricity\\Transient\\";
                        QString trans_fname=baseDirectory + "HyFem\\Piezo_Electricity\\Transient\\DATA"+pe_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<pe_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "HyFem\\Piezo_Electricity\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<pe_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "HyFem\\Piezo_Electricity\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<pe_trans_tdata;                                                       //write the file
                        filet.close();

                        QFile::copy(sourcer,destine_pe_hyfem+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "HyFem\\Piezo_Electricity\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "HyFem\\Piezo_Electricity\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile filehyfem(storer+"\\parser\\Pathname.dat");
                       if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                       outhyfem<<path_name;                                                       //write the file
                       filehyfem.close();

                       QFile filehyf(storer+"\\parser\\Filename.dat");
                       if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                       outhyf<<fil_name;                                                       //write the file
                       filehyf.close();

                        ui->Alert_Parser->setText("Saved");
                    }

            }else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
            }
            else
            {
                ui->Alert_Parser->setText("Please Enter all the details");
            }
        }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }
}

void MainWindow::on_electrostatic_trans_ok_4_clicked()
{

    if(selection!=0)
    {
        if(ui->pr_trans_ls->text()!=NULL&&ui->pr_trans_perm->text()!=NULL&&ui->pr_trans_eleccon->text()!=NULL&&ui->pr_trans_strt->text()!=NULL&&ui->pr_trans_nd->text()!=NULL&&ui->pr_trans_increment->text()!=NULL&&ui->pr_trans_materialid->text()!=NULL)
        {
            if (rx.exactMatch(ui->pr_trans_ls->text())&&rx.exactMatch(ui->pr_trans_perm->text())&&rx.exactMatch(ui->pr_trans_eleccon->text())&&rx.exactMatch(ui->pr_trans_strt->text())&&rx.exactMatch(ui->pr_trans_nd->text())&&rx.exactMatch(ui->pr_trans_increment->text()))
            {
                QString pr_trans_ls ="\n"+ ui->pr_trans_ls->text()+"\n";
                QString pr_trans_matid=ui->pr_trans_materialid->text();
                QString pr_trans_perm="\n"+ui->pr_trans_perm->text()+"\n";
                QString pr_trans_elecon="\n"+ui->pr_trans_eleccon->text()+"\n";
                QString pr_trans_timestr="\n"+ui->pr_trans_strt->text()+"\n";
                QString pr_trans_timeend="\n"+ui->pr_trans_nd->text()+"\n";
                QString pr_trans_timeinc="\n"+ui->pr_trans_increment->text()+"\n";
                QString pr_trans_ldata="*Load Step"+pr_trans_ls;
                QString pr_trans_tdata="*Time Start:"+pr_trans_timestr+"*Time End:"+pr_trans_timeend+"*Time Increment:"+pr_trans_timeinc;
                QString pr_data="*Material ID"+pr_trans_matid+"\n*Permitivity"+pr_trans_perm+"*Electrical Conductivity"+pr_trans_elecon;
                if(selection == 1)
                {

                    QDir().mkdir(baseDirectory + "NISA\\Piezo_Resistivity");
                    QDir().mkdir(baseDirectory + "NISA\\Piezo_Resistivity\\Transient");
                    QString destine_pr_nisa=baseDirectory+"NISA\\Piezo_Resistivity\\Transient\\";
                    QString trans_fname=baseDirectory + "NISA\\Piezo_Resistivity\\Transient\\DATA"+pr_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pr_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "NISA\\Piezo_Resistivity\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pr_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "NISA\\Piezo_Resistivity\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<pr_trans_tdata;                                                       //write the file
                    filet.close();

                     QFile::copy(sourcer,destine_pr_nisa+QFileInfo(source).fileName());

                     QString path_nam=baseDirectory + "NISA\\Piezo_Resistivity\\Transient";
                     QString path_name=QDir::toNativeSeparators(path_nam);
                     QString file_name=baseDirectory + "NISA\\Piezo_Resistivity\\Transient\\"+QFileInfo(source).fileName();
                     QString fil_name=QDir::toNativeSeparators(file_name);


                     QFile filenisa(storer+"\\parser\\Pathname.dat");
                     if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                     return;                                                         //return
                     QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                     outnisa<<path_name;                                                       //write the file
                     filenisa.close();

                     QFile filenis(storer+"\\parser\\Filename.dat");
                     if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                     return;                                                         //return
                     QTextStream outnis(&filenis);                                         //write the file contents to the file
                     outnis<<fil_name;                                                       //write the file
                     filenis.close();

                    ui->Alert_Parser->setText("Saved");
                }
                else if(selection == 2)
                {

                    QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Resistivity");
                    QDir().mkdir(baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient");

                    QString destine_pr_abaqus=baseDirectory+"ABAQUS\\Piezo_Resistivity\\Transient\\";
                    QString trans_fname=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient\\DATA"+pr_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pr_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pr_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<pr_trans_tdata;                                                       //write the file
                    filet.close();
                    QFile::copy(sourcer,destine_pr_abaqus+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ABAQUS\\Piezo_Resistivity\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                   if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                   outabaqus<<path_name;                                                       //write the file
                   fileabaqus.close();

                   QFile fileaba(storer+"\\parser\\Filename.dat");
                   if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outaba(&fileaba);                                         //write the file contents to the file
                   outaba<<fil_name;                                                       //write the file
                   fileaba.close();
                   ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 4)
                {

                    QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Resistivity");
                    QDir().mkdir(baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient");
                    QString destine_pr_ansys=baseDirectory+"ANSYS\\Piezo_Resistivity\\Transient\\";
                    QString trans_fname=baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient\\DATA"+pr_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pr_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pr_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<pr_trans_tdata;                                                       //write the file
                    filet.close();
                    QFile::copy(sourcer,destine_pr_ansys+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "ANSYS\\Piezo_Resistivity\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile fileansys(storer+"\\parser\\Pathname.dat");
                   if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outansys(&fileansys);                                         //write the file contents to the file
                   outansys<<path_name;                                                       //write the file
                   fileansys.close();

                   QFile fileans(storer+"\\parser\\Filename.dat");
                   if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outans(&fileans);                                         //write the file contents to the file
                   outans<<fil_name;                                                       //write the file
                   fileans.close();


                    ui->Alert_Parser->setText("Saved");
                }

                else if(selection == 6)
                {

                    QDir().mkdir(baseDirectory + "HyFem\\Piezo_Resistivity");
                    QDir().mkdir(baseDirectory + "HyFem\\Piezo_Resistivity\\Transient");
                    QString destine_pr_hyfem=baseDirectory+"HyFem\\Piezo_Resistivity\\Transient\\";
                    QString trans_fname=baseDirectory + "HyFem\\Piezo_Resistivity\\Transient\\DATA"+pr_trans_matid+".dat";
                    QFile file(trans_fname);                                       //opening the file specified
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream out(&file);                                         //write the file contents to the file
                    out<<pr_data;                                                       //write the file
                    file.close();

                    QString trans_lname=baseDirectory + "HyFem\\Piezo_Resistivity\\Transient\\DATA.dat";
                    QFile filel(trans_lname);
                    if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outl(&filel);                                         //write the file contents to the file
                    outl<<pr_trans_ldata;                                                       //write the file
                    filel.close();

                    QString trans_tname=baseDirectory + "HyFem\\Piezo_Resistivity\\Transient\\DATAt.dat";
                    QFile filet(trans_tname);
                    if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                    return;                                                         //return
                    QTextStream outt(&filet);                                         //write the file contents to the file
                    outt<<pr_trans_tdata;                                                       //write the file
                    filet.close();
                    QFile::copy(sourcer,destine_pr_hyfem+QFileInfo(source).fileName());

                    QString path_nam=baseDirectory + "HyFem\\Piezo_Resistivity\\Transient";
                    QString path_name=QDir::toNativeSeparators(path_nam);
                    QString file_name=baseDirectory + "HyFem\\Piezo_Resistivity\\Transient\\"+QFileInfo(source).fileName();
                    QString fil_name=QDir::toNativeSeparators(file_name);


                   QFile filehyfem(storer+"\\parser\\Pathname.dat");
                   if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                   outhyfem<<path_name;                                                       //write the file
                   filehyfem.close();

                   QFile filehyf(storer+"\\parser\\Filename.dat");
                   if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                   return;                                                         //return
                   QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                   outhyf<<fil_name;                                                       //write the file
                   filehyf.close();


                    ui->Alert_Parser->setText("Saved");
                }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please Enter all the details");
        }
    }
    else
    {
        ui->Alert_Parser->setText("Please complete transform first");
    }
}

void MainWindow::on_cse_trans_ok_clicked()
{
    if(selection!=0)
    {
        if(ui->cse_trans_ls->text()!=NULL&&ui->cse_trans_perm->text()!=NULL&&ui->cse_trans_eleccon->text()!=NULL&&ui->cse_trans_strt->text()!=NULL&&ui->cse_trans_nd->text()!=NULL&&ui->cse_trans_increment->text()!=NULL&&ui->cse_trans_materialid->text()!=NULL)
        {
            if (rx.exactMatch(ui->cse_trans_ls->text())&&rx.exactMatch(ui->cse_trans_perm->text())&&rx.exactMatch(ui->cse_trans_eleccon->text())&&rx.exactMatch(ui->cse_trans_strt->text())&&rx.exactMatch(ui->cse_trans_nd->text())&&rx.exactMatch(ui->cse_trans_increment->text()))
            {
                    QString cse_trans_ls ="\n"+ ui->cse_trans_ls->text()+"\n";
                    QString cse_trans_matid=ui->cse_trans_materialid->text();
                    QString cse_trans_perm="\n"+ui->cse_trans_perm->text()+"\n";
                    QString cse_trans_elecon="\n"+ui->cse_trans_eleccon->text()+"\n";
                    QString cse_trans_timestr="\n"+ui->cse_trans_strt->text()+"\n";
                    QString cse_trans_timeend="\n"+ui->cse_trans_nd->text()+"\n";
                    QString cse_trans_timeinc="\n"+ui->cse_trans_increment->text()+"\n";
                    QString cse_trans_tdata="*Time Start:"+cse_trans_timestr+"*Time End:"+cse_trans_timeend+"*Time Increment:"+cse_trans_timeinc;
                    QString cse_trans_ldata="*Load Step"+cse_trans_ls;
                    QString cse_data="*Material ID\n"+cse_trans_matid+"\n*Permitivity"+cse_trans_perm+"*Electrical Conductivity"+cse_trans_elecon;
                    if(selection == 1)
                    {

                        QDir().mkdir(baseDirectory + "NISA\\Coupled_Structure_Electrostatic");
                        QDir().mkdir(baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient");
                        QString destine_cse_nisa=baseDirectory+"NISA\\Coupled_Structure_Electrostatic\\Transient\\";
                        QString trans_fname=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient\\DATA"+cse_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<cse_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<cse_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<cse_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_cse_nisa+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "NISA\\Coupled_Structure_Electrostatic\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                        QFile filenisa(storer+"\\parser\\Pathname.dat");
                        if (!filenisa.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnisa(&filenisa);                                         //write the file contents to the file
                        outnisa<<path_name;                                                       //write the file
                        filenisa.close();

                        QFile filenis(storer+"\\parser\\Filename.dat");
                        if (!filenis.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outnis(&filenis);                                         //write the file contents to the file
                        outnis<<fil_name;                                                       //write the file
                        filenis.close();

                       ui->Alert_Parser->setText("Saved");
                    }
                    else if(selection == 2)
                    {

                        QDir().mkdir(baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic");
                        QDir().mkdir(baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient");
                        QString destine_cse_abaqus=baseDirectory+"ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\";
                        QString trans_fname=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\DATA"+cse_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<cse_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<cse_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<cse_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_cse_abaqus+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileabaqus(storer+"\\parser\\Pathname.dat");
                       if (!fileabaqus.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outabaqus(&fileabaqus);                                         //write the file contents to the file
                       outabaqus<<path_name;                                                       //write the file
                       fileabaqus.close();

                       QFile fileaba(storer+"\\parser\\Filename.dat");
                       if (!fileaba.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outaba(&fileaba);                                         //write the file contents to the file
                       outaba<<fil_name;                                                       //write the file
                       fileaba.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 4)
                    {

                        QDir().mkdir(baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic");
                        QDir().mkdir(baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient");
                        QString destine_cse_ansys=baseDirectory+"ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\";
                        QString trans_fname=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient\\DATA"+cse_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<cse_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<cse_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<cse_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_cse_ansys+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "ANSYS\\Coupled_Structure_Electrostatic\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile fileansys(storer+"\\parser\\Pathname.dat");
                       if (!fileansys.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outansys(&fileansys);                                         //write the file contents to the file
                       outansys<<path_name;                                                       //write the file
                       fileansys.close();

                       QFile fileans(storer+"\\parser\\Filename.dat");
                       if (!fileans.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outans(&fileans);                                         //write the file contents to the file
                       outans<<fil_name;                                                       //write the file
                       fileans.close();

                        ui->Alert_Parser->setText("Saved");
                    }

                    else if(selection == 6)
                    {

                        QDir().mkdir(baseDirectory + "HyFem\\Coupled_Structure_Electrostatic");
                        QDir().mkdir(baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient");
                        QString destine_cse_hyfem=baseDirectory+"ABAQUS\\Coupled_Structure_Electrostatic\\Transient\\";
                        QString trans_fname=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient\\DATA"+cse_trans_matid+".dat";
                        QFile file(trans_fname);                                       //opening the file specified
                        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream out(&file);                                         //write the file contents to the file
                        out<<cse_data;                                                       //write the file
                        file.close();

                        QString trans_lname=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient\\DATA.dat";
                        QFile filel(trans_lname);
                        if (!filel.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outl(&filel);                                         //write the file contents to the file
                        outl<<cse_trans_ldata;                                                       //write the file
                        filel.close();

                        QString trans_tname=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient\\DATAt.dat";
                        QFile filet(trans_tname);
                        if (!filet.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                        return;                                                         //return
                        QTextStream outt(&filet);                                         //write the file contents to the file
                        outt<<cse_trans_tdata;                                                       //write the file
                        filet.close();
                        QFile::copy(sourcer,destine_cse_hyfem+QFileInfo(source).fileName());

                        QString path_nam=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient";
                        QString path_name=QDir::toNativeSeparators(path_nam);
                        QString file_name=baseDirectory + "HyFem\\Coupled_Structure_Electrostatic\\Transient\\"+QFileInfo(source).fileName();
                        QString fil_name=QDir::toNativeSeparators(file_name);


                       QFile filehyfem(storer+"\\parser\\Pathname.dat");
                       if (!filehyfem.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyfem(&filehyfem);                                         //write the file contents to the file
                       outhyfem<<path_name;                                                       //write the file
                       filehyfem.close();

                       QFile filehyf(storer+"\\parser\\Filename.dat");
                       if (!filehyf.open(QIODevice::WriteOnly | QIODevice::Text))         //if it is unable to open the file
                       return;                                                         //return
                       QTextStream outhyf(&filehyf);                                         //write the file contents to the file
                       outhyf<<fil_name;                                                       //write the file
                       filehyf.close();


                        ui->Alert_Parser->setText("Saved");
                    }

            }
            else
            {
                ui->Alert_Parser->setText("Invalid Entry");
            }
            }
            else
            {
                ui->Alert_Parser->setText("Please Enter all the details");
            }
        }
        else
        {
            ui->Alert_Parser->setText("Please complete transform first");
        }
}

//Transient data reset starts here
void MainWindow::on_struct_trans_reset_clicked()
{
    ui->struct_trans_ls->setText("0");
    ui->struct_trans_materialid->setValue(1);
    ui->struct_trans_perm->setText("0");
    ui->struct_trans_eleccon->setText("0");
    ui->struct_trans_strt->setText("0");
    ui->struct_trans_nd->setText("0");
    ui->struct_trans_increment->setText("0");
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_cse_trans_reset_clicked()
{
    ui->cse_trans_strt->setText("0");
    ui->cse_trans_nd->setText("0");
    ui->cse_trans_increment->setText("0");
    ui->cse_trans_perm->setText("0");
    ui->cse_trans_eleccon->setText("0");
    ui->cse_trans_ls->setText("0");
    ui->cse_trans_materialid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_pe_trans_reset_clicked()
{


    ui->pe_trans_ls->setText("0");
    ui->pe_trans_strt->setText("0");
    ui->pe_trans_nd->setText("0");
    ui->pe_trans_increment->setText("0");
    ui->pe_trans_perm->setText("0");
    ui->pe_trans_eleccon->setText("0");
    ui->pe_trans_materialid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_pr_trans_reset_clicked()
{
    ui->pr_trans_ls->setText("0");
    ui->pr_trans_strt->setText("0");
    ui->pr_trans_nd->setText("0");
    ui->pr_trans_increment->setText("0");
    ui->pr_trans_materialid->setValue(1);
    ui->pr_trans_perm->setText("0");
    ui->pr_trans_eleccon->setText("0");
    ui->Alert_Parser->setText("Reset Complete");
}

void MainWindow::on_sf_trans_reset_clicked()
{
    ui->sf_trans_ls->setText("0");
    ui->sf_trans_strt->setText("0");
    ui->sf_trans_nd->setText("0");
    ui->sf_trans_increment->setText("0");
    ui->sf_trans_perm->setText("0");
    ui->sf_trans_eleccon->setText("0");
    ui->sf_trans_materialid->setValue(1);
    ui->Alert_Parser->setText("Reset Complete");
}


void MainWindow::on_online_clicked()
{
   QDesktopServices::openUrl(QUrl("http://www.google.com"));
}

void MainWindow::on_print_pdf_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName("F:\\Shrewti\\testoutput.pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                        QString(), "*.pdf");
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(".pdf");
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
           ui->report->print(&printer);
        }
}

void MainWindow::on_printer_print_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
    {
      ui->report->print(&printer);
        printer.newPage();

        delete dlg;
    }
}

void MainWindow::on_view_structure_clicked()
{

        ui->Alert->setText("View Mode");
        ui->Alert->updateGeometry();
        QString fileName=ui->BrowsePath->text();
        QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
        myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
        QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
        QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
        myFile.close();                                                                                                           //Close the open file
        chk_flag=0;
        ui->Structure_info->setPlainText(line);                                                                                //Display contents of the file
        ui->Structure_info->setReadOnly(true);

}

void MainWindow::on_struct_parse_clicked()
{
    //QString filename=QFileDialog::getOpenFileName(this,tr("Open File"),"C://","All Files(*.*)");
  // QDesktopServices::openUrl(QUrl("file:///"+filename,QUrl::TolerantMode));

    QProcess *myproc = new QProcess(this);
    QDir::setCurrent(storer+"\\parser\\");
    QString program="Par_co_str_elect_st.exe";
    myproc->start(program);


    QProcess::ExitStatus Status = myproc->exitStatus();

    if (Status == 0)
    {
        ui->Alert_Parser->setText("Parsing Complete");
    }
    else
    {
        ui->Alert_Parser->setText("Error !Kindly restart the process");
    }

}

void MainWindow::on_electrostatic_parse_clicked()
{
    QProcess* process = new QProcess();
    QString program = "c:\\windows\\system32\\calc.exe";
    process->start(program);
}

void MainWindow::on_CSE_parse_clicked()
{
    QProcess *myproc = new QProcess(this);
    QDir::setCurrent(storer+"\\parser\\");
    QString program="Par_co_str_elect_st.exe";
    myproc->start(program);


    QProcess::ExitStatus Status = myproc->exitStatus();

    if (Status == 0)
    {
        ui->Alert_Parser->setText("Parsing Complete");
    }
    else
    {
        ui->Alert_Parser->setText("Error !Kindly restart the process");
    }

}

void MainWindow::on_PE_parse_clicked()
{
    QProcess* process = new QProcess();
    QString program = "c:\\windows\\system32\\calc.exe";
    process->start(program);
}

void MainWindow::on_electrostatic_parse_4_clicked()
{
    QProcess* process = new QProcess();
    QString program = "c:\\windows\\system32\\calc.exe";
    process->start(program);
}

void MainWindow::on_electrostatic_parse_5_clicked()
{
    QProcess* process = new QProcess();
    QString program = "c:\\windows\\system32\\calc.exe";
    process->start(program);
}

void MainWindow::on_view_Electrostatic_clicked()
{
    ui->Alert->setText("View Mode");
    ui->Alert->updateGeometry();
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
    myFile.close();                                                                                                           //Close the open file
    chk_flag=0;
    ui->Electrostatic_info->setPlainText(line);                                                                                //Display contents of the file
    ui->Electrostatic_info->setReadOnly(true);

}

void MainWindow::on_view_cse_clicked()
{
    ui->Alert->setText("View Mode");
    ui->Alert->updateGeometry();
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
    myFile.close();                                                                                                           //Close the open file
    chk_flag=0;
    ui->CSE_info->setPlainText(line);                                                                                //Display contents of the file
    ui->CSE_info->setReadOnly(true);

}

void MainWindow::on_view_pe_clicked()
{
    ui->Alert->setText("View Mode");
    ui->Alert->updateGeometry();
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
    myFile.close();                                                                                                           //Close the open file
    chk_flag=0;
    ui->PE_info->setPlainText(line);                                                                                //Display contents of the file
    ui->PE_info->setReadOnly(true);

}

void MainWindow::on_view_pr_clicked()
{
    ui->Alert->setText("View Mode");
    ui->Alert->updateGeometry();
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
    myFile.close();                                                                                                           //Close the open file
    chk_flag=0;
    ui->PR_info->setPlainText(line);                                                                                //Display contents of the file
    ui->PR_info->setReadOnly(true);

}

void MainWindow::on_view_sf_clicked()
{
    ui->Alert->setText("View Mode");
    ui->Alert->updateGeometry();
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                   //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadOnly);                                                                                         //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                          //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                      //ensuring all the text can be read
    myFile.close();                                                                                                           //Close the open file
    chk_flag=0;
    ui->sf_info->setPlainText(line);                                                                                //Display contents of the file
    ui->sf_info->setReadOnly(true);

}

//edit parser

void MainWindow::on_edit_structure_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->Structure_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->Structure_info->setReadOnly(false);
}

void MainWindow::on_edit_Electrostatic_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->Electrostatic_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->Electrostatic_info->setReadOnly(false);
}

void MainWindow::on_edit_cse_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->CSE_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->CSE_info->setReadOnly(false);
}

void MainWindow::on_edit_pe_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->PE_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->PE_info->setReadOnly(false);
}

void MainWindow::on_edit_pr_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->PR_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->PR_info->setReadOnly(false);
}

void MainWindow::on_edit_modal_clicked()
{
    ui->Alert->setText("Edit Mode");
    QString fileName=ui->BrowsePath->text();
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->sf_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->sf_info->setReadOnly(false);
}

void MainWindow::on_analyse_clicked()
{
   if(ui->Structural->isChecked() == true)
   {
       analysis_selection = 1;
   }
   else
   if(ui->Electrostatic_2->isChecked() == true)
   {
       analysis_selection = 2;
   }
   else
   if(ui->cse->isChecked() == true)
   {
       analysis_selection = 3;
   }
   else
   if(ui->pe->isChecked() == true)
   {
       analysis_selection = 4;
   }
   else
   if(ui->pr->isChecked() == true)
   {
       analysis_selection = 5;
   }
   else
   if(ui->sf->isChecked() == true)
   {
       analysis_selection = 6;
   }
   else
   if(ui->mod->isChecked() == true)
   {
       analysis_selection = 7;
   }

   if(analysis_selection==1)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==2)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==3)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==4)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==5)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==6)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
   else
   if(analysis_selection==7)
   {
       myproc = new QProcess(this);
       QDir::setCurrent(storer+"\\analyzers\\");
       program="Jmems.exe";
       myproc->start(program);

       QProcess::ExitStatus Status = myproc->exitStatus();

       if (Status == 0)
       {
           ui->Analysis_Alert->setText("Analysis Complete");
       }
       else
       {
           ui->Analysis_Alert->setText("Error !Kindly restart the process");
       }
   }
}

void MainWindow::on_pushButton_2_clicked()
{
    myproc->kill();

}

void MainWindow::on_SaveAsHelp_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName("F:\\Shrewti\\testoutput.pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                        QString(), "*.pdf");
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(".pdf");
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
           ui->texthelper->print(&printer);
        }
}

void MainWindow::on_pushButton_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
    {
      ui->texthelper->print(&printer);
        printer.newPage();

        delete dlg;
    }
}

void MainWindow::on_printabout_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
    {
      ui->about->print(&printer);
        printer.newPage();

        delete dlg;
    }
}

void MainWindow::on_SaveAsAbout_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName("F:\\Shrewti\\testoutput.pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                        QString(), "*.pdf");
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(".pdf");
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
           ui->about->print(&printer);
        }
}


void MainWindow::on_edit_analysis_clicked()
{
    ui->Analysis_Alert->setText("Edit Mode");
    QString fileName=storer+"\\analyzers\\details.dat";
    QFile myFile(fileName);                                                                                                     //opening the required file using an obect myFile
    myFile.open(QIODevice::ReadWrite);                                                                                          //Opening the File in Read only Mode
    QTextStream textStream(&myFile);                                                                                            //Streaming the contents of the file
    QString line = textStream.readAll();                                                                                        //ensureing all the text can be read
    myFile.close();                                                                                                             //Close the open file
    chk_flag=1;
    ui->Analysis_info->setPlainText(line);                                                                                  //Display contents of the file
    ui->Analysis_info->setReadOnly(false);
}

void MainWindow::on_analysis_graphical_output_clicked()
{
    ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_structure_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_electro_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_cse_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_pe_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_pr_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_graphical_sf_clicked()
{
      ui->FunctionTabs->setCurrentIndex(3);
}

void MainWindow::on_Help_topics_clicked(const QModelIndex &index)
{

    QString qs_input;
    qs_input.append("<qt> <table border=1> <tr> <td> 1 </td> <td> 2 </td> </tr>");
    qs_input.append("<tr> <td> One </td> <td> Two </td> </tr> </table> </qt>");

    if(index.data(Qt::DisplayRole)=="Introduction")
    {
        ui->texthelper->setText(qs_input);
    }
}
