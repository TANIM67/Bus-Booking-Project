#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include<conio.h>
#include<QVector>
#include <QSqlError>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QComboBox>
#include <QApplication>
#include <QLabel>
#include <QFont>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Email_input_2_cursorPositionChanged(int arg1, int arg2);

    void on_Sign_in_button_clicked();

    void on_Sign_up_button_clicked();

    void on_Sign_in_button_2_clicked();

    void on_Sign_up_button_2_clicked();

    void on_Log_out_button_clicked();

    void on_Sign_up_page_customContextMenuRequested(const QPoint &pos);

    void on_Book_seat_button_clicked();

    void on_A1_clicked();

    void on_A2_clicked();

    void on_A3_clicked();

    void on_A4_clicked();

    void on_B1_clicked();

    void on_B2_clicked();

    void on_B3_clicked();

    void on_B4_clicked();

    void on_C1_clicked();

    void on_C2_clicked();

    void on_C3_clicked();

    void on_C4_clicked();

    void on_D1_clicked();

    void on_D2_clicked();

    void on_D3_clicked();

    void on_D4_clicked();

    void on_E1_clicked();

    void on_E2_clicked();

    void on_E3_clicked();

    void on_E4_clicked();

    void on_Back_button_clicked();

    //void on_pushButton_clicked();

    void on_Book_new_bus_button_clicked();

    void on_Show_booked_bus_button_clicked();

    //void on_Back_button_2_clicked();

    void on_Back_option_clicked();

    //void on_Add_new_bus_button_clicked();

    void on_Confirm_button_clicked();

    void on_Return_button_clicked();

    void on_Back_button_2_clicked();

    void on_next_button_clicked();

    void on_ExitButton_clicked();

    void on_Exit_button_clicked();

    void on_Exitbutton_clicked();

    void on_Back_button_3_clicked();

    void on_Confirm_seat_button_clicked();

    //void on_pushButton_clicked();

    void on_DownloadTicket_button_clicked();

    void on_ReturnToMenu_clicked();

    void on_BackButton_clicked();

    void on_AdminButton_clicked();

    void on_AddBusButton_clicked();

    void on_DeleteBus_clicked();

    void on_BackButton_2_clicked();

    void on_DeleteButton_clicked();

    void on_EditMembers_clicked();

    void on_BackButton_3_clicked();

    void on_DeleteMemberButton_clicked();

    void on_AddAdminButton_clicked();

    void on_RemoveAdminButton_clicked();

    void on_EditBus_clicked();

    void on_BackButton_4_clicked();

    void on_BusUpdateButton_clicked();

    void handleTimeout();

    void handleTimeout2();

    void on_SeatPricingButton_clicked();

    void on_BackButton_5_clicked();

    void on_UpdatePrice_clicked();

    void on_StartOfferButton_clicked();

    void on_RemoveOfferButton_clicked();

    void on_UpdateAsTime_clicked();

    void on_UpdateInfoButton_clicked();

    void on_ChangeInfoButton_clicked();

    void on_BackButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer2;
    QString email, password, name, phone, confirm_password, em , ps, ph, seat, code, bus_name, bus_type, extra, payment_method, trans_id, date, start, end, StartTime;
    int flag = 0, a1=0, a2=0, a3=0, a4=0, b1=0, b2=0, b3=0, b4=0, c1=0, c2=0, c3=0, c4=0, d1=0, d2=0, d3=0, d4=0, e1=0, e2=0, e3=0, e4=0, size, count, price;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
