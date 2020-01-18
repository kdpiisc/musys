#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isFirstTime;
      QString baseDirectory1,baseDirectory,sourcer,source;

      QMediaPlayer *player;

private slots:
    void on_Close_clicked();

    void on_Minimize_clicked();

    void on_Restore_clicked();

    void on_Maximize_clicked();

    void on_Browser_clicked();

    void on_View_clicked();

    void on_View_2_clicked();

    void on_Search_clicked();

    void on_Save_clicked();

    void on_SetWorkingDirectory_clicked();

    void on_Selector_clicked();

    void on_Transfer_clicked();

    void on_struct_static_ok_clicked();

    void on_es_static_ok_clicked();

    void on_cse_static_ok_clicked();

    void on_pe_static_ok_clicked();

    void on_pr_static_ok_clicked();

    void on_sf_static_ok_clicked();

    void on_struct_static_reset_clicked();

    void on_es_static_reset_clicked();

    void on_cse_static_reset_clicked();

    void on_pe_static_reset_clicked();

    void on_pr_static_reset_clicked();

    void on_sf_static_reset_clicked();

    void on_modal_trans_ok_clicked();

    void on_struct_trans_ok_clicked();

    void on_pe_trans_ok_clicked();

    void on_electrostatic_trans_ok_4_clicked();

    void on_cse_trans_ok_clicked();

    void on_struct_trans_reset_clicked();

    void on_cse_trans_reset_clicked();

    void on_pe_trans_reset_clicked();

    void on_pr_trans_reset_clicked();

    void on_sf_trans_reset_clicked();

    void on_online_clicked();

    void on_print_pdf_clicked();

    void on_printer_print_clicked();

    void on_view_structure_clicked();

    void on_struct_parse_clicked();

    void on_electrostatic_parse_clicked();

    void on_CSE_parse_clicked();

    void on_PE_parse_clicked();

    void on_electrostatic_parse_4_clicked();

    void on_electrostatic_parse_5_clicked();

    void on_view_Electrostatic_clicked();

    void on_view_cse_clicked();

    void on_view_pe_clicked();

    void on_view_pr_clicked();

    void on_view_sf_clicked();

    void on_edit_structure_clicked();

    void on_edit_Electrostatic_clicked();

    void on_edit_cse_clicked();

    void on_edit_pe_clicked();

    void on_edit_pr_clicked();

    void on_edit_modal_clicked();

    void on_analyse_clicked();

    void on_SaveAsHelp_clicked();

    void on_pushButton_clicked();

    void on_printabout_clicked();

    void on_SaveAsAbout_clicked();

    void on_pushButton_2_clicked();

    void on_edit_analysis_clicked();

    void on_analysis_graphical_output_clicked();

    void on_graphical_structure_clicked();

    void on_graphical_electro_clicked();

    void on_graphical_cse_clicked();

    void on_graphical_pe_clicked();

    void on_graphical_pr_clicked();

    void on_graphical_sf_clicked();

    void on_Help_topics_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    qint64 position;
};

#endif // MAINWINDOW_H
