#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

    // Set the timeout interval in milliseconds (e.g., 1 hour = 3600000 ms)
    int timeoutInterval = 120000; // 2 min for those who booked seat but didn't payed;
    //int timeoutInterval2 = 300000;
    timer->start(timeoutInterval);

    timer2 = new QTimer(this);

    // Connect the timeout signal to your custom slot for the second timer
    connect(timer2, SIGNAL(timeout()), this, SLOT(handleTimeout2()));

    // Set the timeout interval for the second timer in milliseconds
    int timeoutInterval2 = 300000; // Adjust as needed
    timer2->start(timeoutInterval2);
    db = QSqlDatabase :: addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/HP/Documents/sample/data.db");
    db.open();
    ui -> stackedWidget -> setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::handleTimeout()
{
    code = "0";
    extra = " ";
    QSqlQuery qr;
    qr.exec("update bengal set code = '"+code+"' , customer = '"+extra+"' where paid = '0'");

    qDebug() << "Timeout occurred! Database action performed.";
}
void MainWindow::handleTimeout2()
{
    code = "0";
    extra = " ";
    QSqlQuery qr;
    qr.exec("update bengal set code = '"+code+"' , customer = '"+extra+"', paid = '0' where paid = '1'");

    qDebug() << "Timeout occurred! Database action performed for all.";
}

void MainWindow::on_Email_input_2_cursorPositionChanged(int arg1, int arg2)
{

}


void MainWindow::on_Sign_in_button_clicked()
{
    em = ui -> Email_input -> text();
    ps = ui -> pass_input -> text();

    QSqlQuery qr;
    qr.exec("SELECT * FROM tanim WHERE  email = '"+em+"'AND pass = '"+ps+"'");
    if(qr.next())
    {
        qr.exec("SELECT * FROM tanim WHERE  email = '"+em+"'AND pass = '"+ps+"'");
        if(qr.next())
        {
            name = qr.value("name").toString();
        }
        ui -> stackedWidget -> setCurrentIndex(4);
        qr.exec("SELECT * FROM tanim WHERE  email = '"+em+"'AND Admin = '0'");
        if(qr.next())
        {
            ui->AdminButton->hide();
        }
        qr.exec("SELECT * FROM tanim WHERE  email = '"+em+"'AND Admin = '1'");
        if(qr.next())
        {
            ui->AdminButton->show();
        }
        qr.exec("SELECT * FROM Offer WHERE  Running = '1'");
        if(qr.next())
        {
            ui->DiscountDiclare->show();
        }
        else
        {
            ui->DiscountDiclare->hide();
        }
        QString offer;

        qr.exec("SELECT adver FROM Offer WHERE Running = '1'");
        if(qr.next())
        {
            offer = qr.value("adver").toString();
        }
        ui->TheOffer->setText(offer);
        QFont font;
        font.setBold(true);
        font.setPointSize(18);
        ui->TheOffer->setFont(font);


        ui-> customer -> setText(name);
        //QFont font;
        font.setPointSize(48);
        font.setBold(true);

        ui->customer->setFont(font);
    }
    else
    {
        QMessageBox::warning(nullptr, "error!", "wrong email or password!");
    }
    ui -> Email_input -> clear();
    ui -> pass_input -> clear();
}


void MainWindow::on_Sign_up_button_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(3);
    ui -> Email_input -> clear();
    ui -> pass_input -> clear();
}


void MainWindow::on_Sign_in_button_2_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
    ui -> Email_input -> clear();
    ui -> pass_input -> clear();

}


void MainWindow::on_Sign_up_button_2_clicked()
{

    email = ui -> Email_input_2 -> text();
    password = ui -> Password_input -> text();
    name = ui -> Name_input -> text();
    phone = ui -> Phone_input -> text();
    confirm_password = ui -> Confirm_pass_input -> text();
    extra = "@";
    QSqlQuery qr;
    qr.exec("SELECT * FROM tanim WHERE  email = '"+email+"'");
    if(qr.next())
    {
        flag = 0;
        QMessageBox::warning(nullptr, "error!", "This email is already registerd");
        return;
    }

    else if(!(email.contains(extra) && email.contains(".com")))
    {
        flag = 0;
        QMessageBox::warning(nullptr, "error!", "Email Not Available!");
        return;
    }
    /*else if(email == "")
    {
        QMessageBox::warning(nullptr, "error!", "Email can't be empty");
    }*/
    else flag ++;
    qr.exec("SELECT * FROM tanim WHERE  phone = '"+phone+"'");
    if(qr.next())
    {
        flag = 0;
        QMessageBox::warning(nullptr, "error!", "This phone number is already registerd");
        return;
    }
    else if(phone.size() != 11)
    {
        flag = 0;
        QMessageBox::warning(nullptr, "error!", "Invalid Phone Number!");
        return;
    }
    else flag++;
    if(password != confirm_password)
    {
        flag = 0;
        QMessageBox::warning(nullptr, "error!", "Password and Confirm password aren't same!");
        return;
    }
    else flag++;
    if(flag == 3 && (email != QString("") && name !=QString("") && password != QString("") && phone!=QString("")))
    {
        flag = 0;
        QMessageBox msgBox;
        msgBox.setText("Succesfully Signed Up!");
        msgBox.exec();
        qr.exec("INSERT INTO tanim (name, email, phone, pass, Admin) VALUES ('"+name+"', '"+email+"', '"+phone+"', '"+password+"', '0') ");
        ui -> stackedWidget -> setCurrentIndex(0);
        ui -> Name_input -> clear();
        ui -> Email_input_2 -> clear();
        ui -> Phone_input -> clear();
        ui -> Password_input -> clear();
        ui -> Confirm_pass_input -> clear();

    }
}


void MainWindow::on_Log_out_button_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(0);
}


void MainWindow::on_Sign_up_page_customContextMenuRequested(const QPoint &pos)
{

}


void MainWindow::on_Book_seat_button_clicked()
{
     //ui -> stackedWidget -> setCurrentIndex(2);
    QSqlQuery quer;
    quer.exec("SELECT * FROM payment WHERE [Bus Type] = 'AC'");
    while (quer.next())
    {
        price= quer.value("TAKA").toInt();

        break;

    }
    ui->ACprice->setText(QString::number(price));

    quer.exec("SELECT * FROM payment WHERE [Bus Type] = 'NON AC'");
    while (quer.next())
    {
        price= quer.value("TAKA").toInt();

        break;

    }
    ui->NonACprice->setText(QString::number(price));

    a1 = 0, a2 = 0, a3 = 0, a4 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    c1 = 0, c2 = 0, c3 = 0, c4 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    e1 = 0, e2 = 0, e3 = 0, e4 = 0;
    // initial coloring of seats depending on their status (either booked or not)

    code = "1";
    bus_name = ui -> select_bus -> currentText();

    QSqlQuery qr;
    // checking if a1 seat is booked by the logged in person. If it is booked by the logged in person
    //      then the seat will be green. if it isn't booked by the logged in person but other person then
    //      the seat will be red means the seat is already booked by someone else
    seat = "a1";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A1 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> A1 -> setStyleSheet("background-color: red");
    }
    // if the seat still isn't booked by anyone then this will make the seat white color

    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A1 -> setStyleSheet("background-color: white");
    }

    // same process for a2 seat
    seat = "a2";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {

        ui -> A2 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> A2 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A2 -> setStyleSheet("background-color: white");
    }


    seat = "a3";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A3 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> A3 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A3 -> setStyleSheet("background-color: white");
    }


    seat = "a4";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A4 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> A4 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> A4 -> setStyleSheet("background-color: white");
    }


    seat = "b1";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B1 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> B1 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B1 -> setStyleSheet("background-color: white");
    }


    seat = "b2";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B2 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> B2 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B2 -> setStyleSheet("background-color: white");
    }


    seat = "b3";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B3 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> B3 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B3 -> setStyleSheet("background-color: white");
    }


    seat = "b4";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B4 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> B4 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> B4 -> setStyleSheet("background-color: white");
    }


    seat = "c1";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C1 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> C1 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C1 -> setStyleSheet("background-color: white");
    }


    seat = "c2";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C2 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> C2 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C2 -> setStyleSheet("background-color: white");
    }


    seat = "c3";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C3 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> C3 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C3 -> setStyleSheet("background-color: white");
    }


    seat = "c4";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C4 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> C4 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> C4 -> setStyleSheet("background-color: white");
    }


    seat = "d1";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D1 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> D1 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D1 -> setStyleSheet("background-color: white");
    }


    seat = "d2";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D2 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> D2 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D2 -> setStyleSheet("background-color: white");
    }


    seat = "d3";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D3 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> D3 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D3 -> setStyleSheet("background-color: white");
    }


    seat = "d4";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D4 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> D4 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> D4 -> setStyleSheet("background-color: white");
    }


    seat = "e1";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E1 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> E1 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E1 -> setStyleSheet("background-color: white");
    }


    seat = "e2";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E2 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> E2 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E2 -> setStyleSheet("background-color: white");
    }


    seat = "e3";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E3 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> E3 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E3 -> setStyleSheet("background-color: white");
    }


    seat = "e4";
    qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E4 -> setStyleSheet("background-color: green");
    }
    else
    {
        ui -> E4 -> setStyleSheet("background-color: red");
    }
    qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        ui -> E4 -> setStyleSheet("background-color: white");
    }

    if(bus_name == "")
    {
        QMessageBox::warning(nullptr, "error!", "You Haven't Booked Any Bus Yet!");
    }
    else ui -> stackedWidget -> setCurrentIndex(2);
    ui -> select_bus -> clear();
}


void MainWindow::on_A1_clicked()
{
    seat = "a1";
    code = "0";
    QSqlQuery qr;
    // checking if the seat is available
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next()) // if the seat is available
    {
        a1++;
        if(a1 % 2 != 0)
        {
            ui -> A1 -> setStyleSheet("background-color: green");
            QSqlDatabase :: database().transaction();
            //QSqlQuery Query_Update_Data (db);
            qr.prepare ("UPDATE bengal SET code = :code, customer = :customer where seat = :seat and bus_name = :bus_name");
            qr.bindValue(":code", "1");
            qr.bindValue(":seat","a1");
            qr.bindValue(":customer", em);
            qr.bindValue(":bus_name", bus_name);
            qr.exec();
            QSqlDatabase :: database().commit();
            //db.close();
            if (!qr.exec()) {
                qDebug() << "Error: " << qr.lastError().text();
            }


        }
        else
        {
            a1 = 0;
            ui -> A1 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> A1 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            a1++;
            if(a1 % 2 != 0)
            {
                ui -> A1 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                a1 = 0;
                extra = " ";
                ui -> A1 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}
void MainWindow::on_A2_clicked()
{
    seat = "a2";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        a2++;
        if(a2 % 2 != 0)
        {
            code = "1";
            ui -> A2 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            a2 = 0;
            ui -> A2 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> A2 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            a2++;
            if(a2 % 2 != 0)
            {
                ui -> A2 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                a2 = 0;
                extra = " ";
                ui -> A2 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }

}


void MainWindow::on_A3_clicked()
{
    seat = "a3";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        a3++;
        if(a3 % 2 != 0)
        {
            code = "1";
            ui -> A3 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            a3 = 0;
            ui -> A3 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> A3 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            a3++;
            if(a3 % 2 != 0)
            {
                ui -> A3 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                a3 = 0;
                extra = " ";
                ui -> A3 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_A4_clicked()
{
    seat = "a4";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        a4++;
        if(a4 % 2 != 0)
        {
            code = "1";
            ui -> A4 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            a4 = 0;
            ui -> A4 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> A4 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            a4++;
            if(a4 % 2 != 0)
            {
                ui -> A4 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                a4 = 0;
                extra = " ";
                ui -> A4 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_B1_clicked()
{
    seat = "b1";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        b1++;
        if(b1 % 2 != 0)
        {
            code = "1";
            ui -> B1 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            b1 = 0;
            ui -> B1 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> B1 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            b1++;
            if(b1 % 2 != 0)
            {
                ui -> B1 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                b1 = 0;
                extra = " ";
                ui -> B1 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_B2_clicked()
{
    seat = "b2";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        b2++;
        if(b2 % 2 != 0)
        {
            code = "1";
            ui -> B2 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            b2 = 0;
            ui -> B2 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> B2 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            b2++;
            if(b2 % 2 != 0)
            {
                ui -> B2 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                b2 = 0;
                extra = " ";
                ui -> B2 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_B3_clicked()
{
    seat = "b3";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        b3++;
        if(b3 % 2 != 0)
        {
            code = "1";
            ui -> B3 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            b3 = 0;
            ui -> B3 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> B3 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            b3++;
            if(b3 % 2 != 0)
            {
                ui -> B3 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                b3 = 0;
                extra = " ";
                ui -> B3 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_B4_clicked()
{
    seat = "b4";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        b4++;
        if(b4 % 2 != 0)
        {
            code = "1";
            ui -> B4 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            b4 = 0;
            ui -> B4 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> B4 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            b4++;
            if(b4 % 2 != 0)
            {
                ui -> B4 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                b4 = 0;
                extra = " ";
                ui -> B4 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_C1_clicked()
{
    seat = "c1";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        c1++;
        if(c1 % 2 != 0)
        {
            code = "1";
            ui -> C1 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            c1 = 0;
            ui -> C1 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> C1 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            c1++;
            if(c1 % 2 != 0)
            {
                ui -> C1 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                c1 = 0;
                extra = " ";
                ui -> C1 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_C2_clicked()
{
    seat = "c2";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        c2++;
        if(c2 % 2 != 0)
        {
            code = "1";
            ui -> C2 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            c2 = 0;
            ui -> C2 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> C2 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            c2++;
            if(c2 % 2 != 0)
            {
                ui -> C2 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                c2 = 0;
                extra = " ";
                ui -> C2 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_C3_clicked()
{
    seat = "c3";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        c3++;
        if(c3 % 2 != 0)
        {
            code = "1";
            ui -> C3 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        }
        else
        {
            c3 = 0;
            ui -> C3 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> C3 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            c3++;
            if(c3 % 2 != 0)
            {
                ui -> C3 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                c3 = 0;
                extra = " ";
                ui -> C3 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_C4_clicked()
{
    seat = "c4";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        c4++;
        if(c4 % 2 != 0)
        {
            code = "1";
            ui -> C4 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            c4 = 0;
            ui -> C4 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> C4 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            c4++;
            if(c4 % 2 != 0)
            {
                ui -> C4 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                c4 = 0;
                extra = " ";
                ui -> C4 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_D1_clicked()
{
    seat = "d1";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        d1++;
        if(d1 % 2 != 0)
        {
            code = "1";
            ui -> D1 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        }
        else
        {
            d1 = 0;
            ui -> D1 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> D1 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            d1++;
            if(d1 % 2 != 0)
            {
                ui -> D1 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                d1 = 0;
                extra = " ";
                ui -> D1 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_D2_clicked()
{
    seat = "d2";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        d2++;
        if(d2 % 2 != 0)
        {
            code = "1";
            ui -> D2 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        }
        else
        {
            d2 = 0;
            ui -> D2 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> D2 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            d2++;
            if(d2 % 2 != 0)
            {
                ui -> D2 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                d2 = 0;
                extra = " ";
                ui -> D2 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_D3_clicked()
{
    seat = "d3";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        d3++;
        if(d3 % 2 != 0)
        {
            code = "1";
            ui -> D3 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        }
        else
        {
            d3 = 0;
            ui -> D3 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> D3 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            d3++;
            if(d3 % 2 != 0)
            {
                ui -> D3 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                d3 = 0;
                extra = " ";
                ui -> D3 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_D4_clicked()
{
    seat = "d4";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        d4++;
        if(d4 % 2 != 0)
        {
            code = "1";
            ui -> D4 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            d4 = 0;
            ui -> D4 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> D4 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");
        }
        else
        {
            d4++;
            if(d4 % 2 != 0)
            {
                ui -> D4 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                d4 = 0;
                extra = " ";
                ui -> D4 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_E1_clicked()
{
    seat = "e1";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        e1++;
        if(e1 % 2 != 0)
        {
            code = "1";
            ui -> E1 -> setStyleSheet("background-color: green");
           qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            e1 = 0;
            ui -> E1 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> E1 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            e1++;
            if(e1 % 2 != 0)
            {
                ui -> E1 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                e1 = 0;
                extra = " ";
                ui -> E1 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_E2_clicked()
{
    seat = "e2";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        e2++;
        if(e2 % 2 != 0)
        {
            code = "1";
            ui -> E2 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            e2 = 0;
            ui -> E2 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> E2 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            e2++;
            if(e2 % 2 != 0)
            {
                ui -> E2 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                e2 = 0;
                extra = " ";
                ui -> E2 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_E3_clicked()
{
    seat = "e3";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        e3++;
        if(e3 % 2 != 0)
        {
            code = "1";
            ui -> E3 -> setStyleSheet("background-color: green");
            qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            e3 = 0;
            ui ->   E3 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> E3 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
           e3++;
            if(e3 % 2 != 0)
            {
                ui -> E3 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                e3 = 0;
                extra = " ";
                ui -> E3 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_E4_clicked()
{
    seat = "e4";
    code = "0";
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal WHERE code = '"+code+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        e4++;
        if(e4 % 2 != 0)
        {
            code = "1";
            ui -> E4 -> setStyleSheet("background-color: green");
           qr.exec("update bengal set code = '"+code+"' , customer = '"+em+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");

        }
        else
        {
            e4 = 0;
            ui -> E4 -> setStyleSheet("background-color: white");
        }
    }
    else // if the seat isn't available
    {
        // checking if the logged in customer booked it
        qr.exec("SELECT * FROM bengal WHERE customer = '"+em+"'AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(!qr.next())
        {
            ui -> E4 -> setStyleSheet("background-color: red");
            QMessageBox::warning(nullptr, "error!", "This seat isn't available!");

        }
        else
        {
            e4++;
            if(e4 % 2 != 0)
            {
                ui -> E4 -> setStyleSheet("background-color: green");
                QMessageBox::warning(nullptr, "reminder!", "You already booked it!");

            }
            else
            {
                e4 = 0;
                extra = " ";
                ui -> E4 -> setStyleSheet("background-color: white");
                code = "0";
                QMessageBox::warning(nullptr, "warning!", "Booking canceled!");
                qr.exec("update bengal set code = '"+code+"', customer = '"+extra+"' where seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
            }
        }
    }
}


void MainWindow::on_Back_button_clicked()
{

    ui -> stackedWidget -> setCurrentIndex(4);


    ui-> get_bus_name -> clear();


    /*QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal WHERE customer = '" + em + "'");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
        QString bus_name = qr.value("bus_name").toString();

        // Check if the bus name is not already in the set before adding it to the combo box
        if (!uniqueBusNames.contains(bus_name))
        {
            ui-> get_bus_name-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
        }
    }*/

}

void MainWindow::on_Book_new_bus_button_clicked()
{
    //flag = 2;
    code = "0";
    // to take available buses name in combo box;
    ui-> select_bus -> clear();

    ui->stackedWidget->setCurrentIndex(1);
    QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal WHERE code = '" + code + "'");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
        QString bus_name = qr.value("bus_name").toString();

        // Check if the bus name is not already in the set before adding it to the combo box
        if (!uniqueBusNames.contains(bus_name))
        {
            ui-> select_bus-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
        }
    }

    // to show available buses in table view;
    QSqlQueryModel *var = new QSqlQueryModel(ui->available_bus);
    QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal WHERE code = '"+code+"'");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));


    ui->available_bus->setModel(var);

    //qDebug() << flag;


}


void MainWindow::on_Show_booked_bus_button_clicked()
{
    //flag = 1;
    ui -> stackedWidget -> setCurrentIndex(9);

    ui-> get_bus_name -> clear();

    // adding booked buses by the logged in person in combo box;
    QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal WHERE customer = '" + em + "'");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
        QString bus_name = qr.value("bus_name").toString();

        // Check if the bus name is not already in the set before adding it to the combo box
        if (!uniqueBusNames.contains(bus_name))
        {
            ui-> get_bus_name-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
        }
    }
    // showing booked buses by the logged in person in table view;
    QSqlQueryModel *var = new QSqlQueryModel(ui->BookedBuses);
    QString query = QString("SELECT DISTINCT customer, bus_name, bus_type, start, end, Stime FROM bengal WHERE customer = '"+em+"'");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("customer"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(5, Qt::Horizontal, QObject::tr("StartTime"));

    ui->BookedBuses->setModel(var);

}

void MainWindow::on_Back_option_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}

    /*s[0] = "a1"; s[1] = "a2"; s[2] = "a3"; s[3] = "a4";
    s[4] = "b1"; s[5] = "b2"; s[6] = "b3"; s[7] = "b4";
    s[8] = "c1"; s[9] = "c2"; s[10] = "c3"; s[11] = "c4";
    s[12] = "d1"; s[13] = "d2"; s[14] = "d3"; s[15] = "d4";
    s[16] = "e1"; s[17] = "e2"; s[18] = "e3"; s[19] = "e4";*/


void MainWindow::on_Confirm_button_clicked()
{
    QSqlQuery qr;
    bus_name = ui -> Bus_name -> text();
    bus_type = ui -> ac_or_non_ac -> currentText();
    start = ui -> from -> text();
    end = ui -> to -> text();
    code = "0";
    extra = " ";
    StartTime = ui -> StartTime -> text();
    if(bus_name == "" || start == "" || end == "")
    {
        QMessageBox::warning(nullptr, "error!", "Please Fill All The Blanks!");
        return;
    }
    qr.exec("SELECT * FROM bengal WHERE  bus_name = '"+bus_name+"'");
    if(qr.next())
    {
        QMessageBox::warning(nullptr, "error!", "Already Registered Bus!");
        return;
    }
    qr.exec("SELECT * FROM bengal WHERE start = '"+start+"' AND end = '"+end+"' AND Stime = '"+StartTime+"'");
    if(qr.next())
    {
        QMessageBox::warning(nullptr, "error!", "Already a bus is here in same route and time!");
        return;
    }
    ui -> Bus_name -> clear();
    ui -> from -> clear();
    ui -> to -> clear();
    QVector <QString> s (20);
    s[0] = "a1"; s[1] = "a2"; s[2] = "a3"; s[3] = "a4";
    s[4] = "b1"; s[5] = "b2"; s[6] = "b3"; s[7] = "b4";
    s[8] = "c1"; s[9] = "c2"; s[10] = "c3"; s[11] = "c4";
    s[12] = "d1"; s[13] = "d2"; s[14] = "d3"; s[15] = "d4";
    s[16] = "e1"; s[17] = "e2"; s[18] = "e3"; s[19] = "e4";
    QSqlDatabase::database().transaction();


    qr.exec("INSERT INTO bengal (seat, code, customer, [bus_name], [bus_type], start, end, Stime, paid) VALUES ('"+s[0]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, [bus_name], [bus_type], start, end, Stime, paid) VALUES ('"+s[1]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[2]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[3]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[4]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[5]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[6]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[7]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[8]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[9]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[10]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[11]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[12]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[13]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[14]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[15]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[16]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[17]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[18]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");
    qr.exec("INSERT INTO bengal (seat, code, customer, bus_name, bus_type, start, end, Stime, paid) VALUES ('"+s[19]+"', '"+code+"', '"+extra+"', '"+bus_name+"', '"+bus_type+"', '"+start+"','"+end+"', '"+StartTime+"', '0') ");


    QSqlDatabase::database().commit();
    QMessageBox msgBox;
    msgBox.setText("Bus Added!");
    msgBox.exec();

    ui -> Bus_name -> clear();
    ui -> from -> clear();
    ui -> to -> clear();
}


void MainWindow::on_Return_button_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(8);
}

void MainWindow::on_Back_button_2_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}


void MainWindow::on_next_button_clicked()
{
    QSqlQuery quer;
    quer.exec("SELECT * FROM payment WHERE [Bus Type] = 'AC'");
    while (quer.next())
    {
        price= quer.value("TAKA").toInt();

        break;

    }
    ui->ACprice->setText(QString::number(price));

    quer.exec("SELECT * FROM payment WHERE [Bus Type] = 'NON AC'");
    while (quer.next())
    {
        price= quer.value("TAKA").toInt();

        break;

    }
    ui->NonACprice->setText(QString::number(price));

    a1 = 0, a2 = 0, a3 = 0, a4 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    c1 = 0, c2 = 0, c3 = 0, c4 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    e1 = 0, e2 = 0, e3 = 0, e4 = 0;
    // initial coloring of seats depending on their status (either booked or not)

    code = "1";
    bus_name = ui -> get_bus_name -> currentText();

    QSqlQuery qr;
    // checking if a1 seat is booked by the logged in person. If it is booked by the logged in person
    //      then the seat will be green. if it isn't booked by the logged in person but other person then
    //      the seat will be red means the seat is already booked by someone else
        seat = "a1";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A1 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> A1 -> setStyleSheet("background-color: red");
        }
        // if the seat still isn't booked by anyone then this will make the seat white color

        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A1 -> setStyleSheet("background-color: white");
        }

        // same process for a2 seat
        seat = "a2";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {

            ui -> A2 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> A2 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A2 -> setStyleSheet("background-color: white");
        }


        seat = "a3";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A3 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> A3 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A3 -> setStyleSheet("background-color: white");
        }


        seat = "a4";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A4 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> A4 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> A4 -> setStyleSheet("background-color: white");
        }


        seat = "b1";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B1 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> B1 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B1 -> setStyleSheet("background-color: white");
        }


        seat = "b2";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B2 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> B2 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B2 -> setStyleSheet("background-color: white");
        }


        seat = "b3";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B3 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> B3 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B3 -> setStyleSheet("background-color: white");
        }


        seat = "b4";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B4 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> B4 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> B4 -> setStyleSheet("background-color: white");
        }


        seat = "c1";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C1 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> C1 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C1 -> setStyleSheet("background-color: white");
        }


        seat = "c2";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C2 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> C2 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C2 -> setStyleSheet("background-color: white");
        }


        seat = "c3";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C3 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> C3 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C3 -> setStyleSheet("background-color: white");
        }


        seat = "c4";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C4 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> C4 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> C4 -> setStyleSheet("background-color: white");
        }


        seat = "d1";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D1 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> D1 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D1 -> setStyleSheet("background-color: white");
        }


        seat = "d2";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D2 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> D2 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D2 -> setStyleSheet("background-color: white");
        }


        seat = "d3";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D3 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> D3 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D3 -> setStyleSheet("background-color: white");
        }


        seat = "d4";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D4 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> D4 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> D4 -> setStyleSheet("background-color: white");
        }


        seat = "e1";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E1 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> E1 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E1 -> setStyleSheet("background-color: white");
        }


        seat = "e2";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E2 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> E2 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E2 -> setStyleSheet("background-color: white");
        }


        seat = "e3";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E3 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> E3 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E3 -> setStyleSheet("background-color: white");
        }


        seat = "e4";
        qr.exec("SELECT * FROM bengal WHERE  code = '"+code+"' AND customer = '"+em+"' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E4 -> setStyleSheet("background-color: green");
        }
        else
        {
            ui -> E4 -> setStyleSheet("background-color: red");
        }
        qr.exec("SELECT * FROM bengal WHERE  code = '0' AND seat = '"+seat+"' AND bus_name = '"+bus_name+"'");
        if(qr.next())
        {
            ui -> E4 -> setStyleSheet("background-color: white");
        }

        if(bus_name == "")
        {
            QMessageBox::warning(nullptr, "error!", "You Haven't Booked Any Bus Yet!");
        }
        else ui -> stackedWidget -> setCurrentIndex(2);
        ui -> get_bus_name -> clear();
}


void MainWindow::on_ExitButton_clicked()
{
        // to end the program;

            QApplication::exit(0);

}


void MainWindow::on_Exit_button_clicked()
{
    // to end the program;

            QApplication::exit(0);
}


void MainWindow::on_Exitbutton_clicked()
{
            // to end the program;

            QApplication::exit(0);
}


void MainWindow::on_Back_button_3_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(2);
}


void MainWindow::on_Confirm_seat_button_clicked()
{
    flag = 0;
    QSqlQuery qr;
    qr.exec("SELECT seat FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");

    while (qr.next())
    {
            //seat = ui -> From -> text();
            //seat+= " ";
            extra= qr.value("start").toString();
            ui-> From-> setText(extra);
            flag = 1;
            break;

    }
    if(flag == 0)
    {
            QMessageBox::warning(nullptr, "error!", "You have no booked seat in this BUS!");
            return;
    }
    ui -> stackedWidget -> setCurrentIndex(6);
    // colouring the Payment_page based on the payment method selected from payment_select options;

    payment_method = ui -> payment_select -> currentText();
    if(payment_method == "BKash")
    {
            //ui -> Payment_page -> setStyleSheet("background-color: rgb(255, 0, 127)"); // pink colour
            ui -> method_name -> setStyleSheet("background-color: rgb(255, 0, 127)"); // pink colour
            ui->method_name->setText(payment_method);
            QFont font;
            font.setPointSize(16);
            ui->method_name->setFont(font);

    }
    else if(payment_method == "Nagad")
    {
             //ui -> Payment_page -> setStyleSheet("background-color: rgb(255, 55, 58)"); // red colour
            ui -> method_name -> setStyleSheet("background-color: rgb(255, 55, 58)"); // red colour
            ui->method_name->setText(payment_method);
            QFont font;
            font.setPointSize(16);
            ui->method_name->setFont(font);
    }
    else if(payment_method == "Rocket")
    {
            // ui -> Payment_page -> setStyleSheet("background-color: rgb(85, 0, 127)"); // blue colour
            ui -> method_name -> setStyleSheet("background-color: rgb(85, 0, 127)"); // blue colour
            ui->method_name->setText(payment_method);
            QFont font;
            font.setPointSize(16);
            ui->method_name->setFont(font);
    }
    else if(payment_method == "Cash")
    {

            ui -> method_name -> setStyleSheet("background-color: rgb(157, 157, 157)"); // grey colour
            ui->method_name->setText(payment_method);
            QFont font;
            font.setPointSize(16);
            ui->method_name->setFont(font);
    }
    flag = 0;
    qr.exec("SELECT bus_type FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");
    while (qr.next())
    {
            extra= qr.value("bus_type").toString();
            break;
    }
    qr.exec("SELECT * FROM payment WHERE [Bus Type] = '" + extra+ "'");
    while (qr.next())
    {
            price= qr.value("TAKA").toInt();

            break;

    }
    count = 0;
    qr.exec("SELECT * FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");
    while (qr.next())
    {
            count++;
    }
    price = price * count;
    ui->Taka->setText(QString::number(price));
    QFont font;
    font.setBold(true);
    font.setPointSize(16);
    ui->Taka->setFont(font);
}



void MainWindow::on_DownloadTicket_button_clicked()
{

    ph = ui -> pay_num -> text();
    trans_id = ui ->trans_id -> text();
    if(ph == "" || ph.size() != 11)
    {
            QMessageBox::warning(nullptr, "error!", "Invalid Phone Number!");
            //ui -> pay_num -> clear();
            return;
    }
    if(trans_id.size() != 6)
    {
            QMessageBox::warning(nullptr, "error!", "Invalid Transaction ID!");
            //ui -> trans_id -> clear();
            return;
    }

    ui -> trans_id -> clear();
    ui -> pay_num -> clear();
    ui->CustomerName->setText(em);
    ui->BusName->setText(bus_name);


    QSqlQuery qr;
    qr.exec("update bengal set paid = '1' where customer = '"+em+"' AND bus_name = '"+bus_name+"'");

    qr.exec("SELECT seat FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");

    //QSet<QString> seatNames; // Set to store seat names

    while (qr.next())
    {
            seat = ui -> seats -> text();
            seat+= " ";
            seat += qr.value("seat").toString();
            ui-> seats-> setText(seat);


    }
    qr.exec("SELECT start FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");

    while (qr.next())
    {
            extra= qr.value("start").toString();
            ui-> From-> setText(extra);
            break;

    }
    qr.exec("SELECT end FROM bengal WHERE customer = '" + em + "' AND bus_name = '"+bus_name+"'");

    while (qr.next())
    {
            extra= qr.value("end").toString();
            ui-> To -> setText(extra);
            break;

    }
    date = ui -> ticket_date -> text();
    ui-> Date -> setText(date);

    QString paid = ui -> total_amount -> text();
    int paidAmount = paid.toInt();

    if(paidAmount != price)
    {
            QMessageBox::warning(nullptr, "error!", "Please Pay the given amount!!");
            return;
    }
    ui -> total_amount -> clear();

    // Set the font size to 16, make it bold and italic
    QFont font;
    font.setPointSize(14);
    font.setBold(true);
    font.setItalic(true);

    // Apply the font to the QLabel
    ui->CustomerName->setFont(font);
    ui->BusName->setFont(font);
    ui->seats->setFont(font);
    ui->From->setFont(font);
    ui->To->setFont(font);
    ui->Date->setFont(font);

    ui -> stackedWidget -> setCurrentIndex(7);

}


void MainWindow::on_ReturnToMenu_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}


void MainWindow::on_BackButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}


void MainWindow::on_AdminButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(8);
}


void MainWindow::on_AddBusButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(5);
}


void MainWindow::on_DeleteBus_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(10);
    ui -> BusCombo -> clear();
    QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
            QString bus_name = qr.value("bus_name").toString();

            // Check if the bus name is not already in the set before adding it to the combo box
            if (!uniqueBusNames.contains(bus_name))
            {
            ui-> BusCombo-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
            }
    }

    QSqlQueryModel *var = new QSqlQueryModel(ui->ShowBus);
    QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal");
    var->setQuery(query);
    //var->setHeaderData(0, Qt::Horizontal, QObject::tr("customer"));
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));

    ui->ShowBus->setModel(var);
}


void MainWindow::on_BackButton_2_clicked()
{
     ui -> stackedWidget -> setCurrentIndex(8);
}


void MainWindow::on_DeleteButton_clicked()
{
    bus_name = ui -> BusCombo -> currentText();
     ui -> BusCombo -> clear();
     QSqlQuery qr;
     qr.exec("DELETE FROM bengal WHERE bus_name = '"+bus_name+"'");
     qr.exec("SELECT bus_name FROM bengal");

     QSet<QString> uniqueBusNames; // Set to store unique bus names
     while (qr.next())
     {
            QString bus_name = qr.value("bus_name").toString();

            // Check if the bus name is not already in the set before adding it to the combo box
            if (!uniqueBusNames.contains(bus_name))
            {
            ui-> BusCombo-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
            }
     }
     QSqlQueryModel *var = new QSqlQueryModel(ui->ShowBus);
     QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal");
     var->setQuery(query);
     var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
     var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
     var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
     var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
     var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));

     ui->ShowBus->setModel(var);
}


void MainWindow::on_EditMembers_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(11);
     QSqlQueryModel *var = new QSqlQueryModel(ui->Members);
     QString query = QString("SELECT DISTINCT name, email, phone, Admin FROM tanim");
     var->setQuery(query);
     var->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
     var->setHeaderData(1, Qt::Horizontal, QObject::tr("Email"));
     var->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone"));
     var->setHeaderData(3, Qt::Horizontal, QObject::tr("Admin"));
     //var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));

     ui->Members->setModel(var);
}


void MainWindow::on_BackButton_3_clicked()
{
     ui -> stackedWidget -> setCurrentIndex(8);
}


void MainWindow::on_DeleteMemberButton_clicked()
{
     QSqlQuery qr;
    extra = ui -> EnterName -> text();
    qr.exec("DELETE FROM tanim WHERE email = '"+extra+"'");
    QSqlQueryModel *var = new QSqlQueryModel(ui->Members);
    QString query = QString("SELECT DISTINCT name, email, phone, Admin FROM tanim");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("Email"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("Admin"));

    ui->Members->setModel(var);

    ui -> EnterName -> clear();
}


void MainWindow::on_AddAdminButton_clicked()
{
    QSqlQuery qr;
    extra = ui -> EnterName -> text();
    qr.exec("update tanim set Admin = '1' WHERE email = '"+extra+"'");
    QSqlQueryModel *var = new QSqlQueryModel(ui->Members);
    QString query = QString("SELECT DISTINCT name, email, phone, Admin FROM tanim");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("Email"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("Admin"));

    ui->Members->setModel(var);

    ui -> EnterName -> clear();
}


void MainWindow::on_RemoveAdminButton_clicked()
{
    QSqlQuery qr;
    extra = ui -> EnterName -> text();
    qr.exec("update tanim set Admin = '0' WHERE email = '"+extra+"'");
    QSqlQueryModel *var = new QSqlQueryModel(ui->Members);
    QString query = QString("SELECT DISTINCT name, email, phone, Admin FROM tanim");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("Email"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("Admin"));

    ui->Members->setModel(var);

    ui -> EnterName -> clear();
}


void MainWindow::on_EditBus_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(12);
    ui -> BusBox -> clear();
    QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
            QString bus_name = qr.value("bus_name").toString();

            // Check if the bus name is not already in the set before adding it to the combo box
            if (!uniqueBusNames.contains(bus_name))
            {
            ui-> BusBox-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
            }
    }

    QSqlQueryModel *var = new QSqlQueryModel(ui->BusEditTable);
    QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal");
    var->setQuery(query);
    //var->setHeaderData(0, Qt::Horizontal, QObject::tr("customer"));
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));

    ui->BusEditTable->setModel(var);
}


void MainWindow::on_BackButton_4_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(8);
}


void MainWindow::on_BusUpdateButton_clicked()
{

    bus_name = ui -> BusBox -> currentText();
    bus_type = ui -> BusType -> currentText();
    start = ui -> FromEdit -> text();
    end = ui -> ToEdit -> text();
    StartTime = ui -> timeEdit -> text();
    if(extra == "" || start == "" || end == "")
    {
            QMessageBox::warning(nullptr, "error!", "Fill All The Blanks!");
            return;
    }
    QSqlQuery qr;
    qr.exec("SELECT * FROM bengal where bus_name = '"+extra+"'");
    if(qr.next())
    {
            QMessageBox::warning(nullptr, "error!", "This Name Isn't Available!");
            return;
    }
    qr.exec("update bengal set bus_type = '"+bus_type+"', start = '"+start+"', end = '"+end+"', Stime = '"+StartTime+"' WHERE bus_name = '"+bus_name+"'");


    QSet<QString> uniqueBusNames; // Set to store unique bus names

    QSqlQuery quer;
    while (quer.next())
    {
            QString bus_name = quer.value("bus_name").toString();

            // Check if the bus name is not already in the set before adding it to the combo box
            if (!uniqueBusNames.contains(bus_name))
            {
            ui-> BusBox-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
            }
    }
    QSqlQueryModel *var = new QSqlQueryModel(ui->BusEditTable);
    QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal");
    var->setQuery(query);
    //var->setHeaderData(0, Qt::Horizontal, QObject::tr("customer"));
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));

    ui->BusEditTable->setModel(var);
    ui -> FromEdit -> clear();
    ui -> ToEdit -> clear();
}


void MainWindow::on_SeatPricingButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(13);
}


void MainWindow::on_BackButton_5_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(8);
}


void MainWindow::on_UpdatePrice_clicked()
{
    bus_type = ui -> TypeSelect -> currentText();
    extra = ui -> EnterNewPrice -> text();
    price = extra.toInt();
    if(extra == "")
    {
            QMessageBox::warning(nullptr, "error!", "Enter New Price!");
            return;
    }
    QSqlQuery qr;
    qr.exec("update payment set TAKA = '"+ QString::number(price) +"' where [Bus Type] = '"+bus_type+"'");
    ui -> EnterNewPrice ->clear();
    QMessageBox msgBox;
    msgBox.setText("Succesfully Updated!");
    msgBox.exec();
}


void MainWindow::on_StartOfferButton_clicked()
{
    extra = ui -> EnterTheOffer -> text();
    if(extra == "")
    {
            QMessageBox::warning(nullptr, "error!", "Enter New Offer!");
            return;
    }
    QSqlQuery qr;
    qDebug() << extra;
    qr.exec("update Offer set adver = '"+extra+"', Running = '1'");
    ui -> EnterTheOffer -> clear();
    QMessageBox msgBox;
    msgBox.setText("Succesfully Started The Advertisment!");
    msgBox.exec();
    QString offer;

    qr.exec("SELECT adver FROM Offer");
    if(qr.next())
    {
            offer = qr.value("adver").toString();
    }
    ui->TheOffer->setText(offer);
    QFont font;
    font.setBold(true);
    font.setPointSize(18);
    ui->TheOffer->setFont(font);

    ui->DiscountDiclare->show();

    ui->TheOffer->show();

}


void MainWindow::on_RemoveOfferButton_clicked()
{
    QSqlQuery qr;
    qr.exec("update Offer set Running = '0'");
    QMessageBox msgBox;
    msgBox.setText("Ad is stopped!");
    msgBox.exec();

    ui->DiscountDiclare->hide();
    ui->TheOffer->hide();
}


void MainWindow::on_UpdateAsTime_clicked()
{
    extra = ui -> StartTime_2 -> text();
    code = "0";
    // to take available buses name in combo box;
    ui-> select_bus -> clear();

    ui->stackedWidget->setCurrentIndex(1);
    QSqlQuery qr;
    qr.exec("SELECT bus_name FROM bengal WHERE code = '" + code + "' AND Stime = '"+extra+"'");

    QSet<QString> uniqueBusNames; // Set to store unique bus names

    while (qr.next())
    {
            QString bus_name = qr.value("bus_name").toString();

            // Check if the bus name is not already in the set before adding it to the combo box
            if (!uniqueBusNames.contains(bus_name))
            {
            ui-> select_bus-> addItem(bus_name);
            uniqueBusNames.insert(bus_name);
            }
    }

    // to show available buses in table view;
    QSqlQueryModel *var = new QSqlQueryModel(ui->available_bus);
    QString query = QString("SELECT DISTINCT bus_name, bus_type, start, end, Stime FROM bengal WHERE code = '"+code+"' AND Stime = '"+extra+"'");
    var->setQuery(query);
    var->setHeaderData(0, Qt::Horizontal, QObject::tr("bus_name"));
    var->setHeaderData(1, Qt::Horizontal, QObject::tr("bus_type"));
    var->setHeaderData(2, Qt::Horizontal, QObject::tr("start"));
    var->setHeaderData(3, Qt::Horizontal, QObject::tr("end"));
    var->setHeaderData(4, Qt::Horizontal, QObject::tr("StartTime"));


    ui->available_bus->setModel(var);
}


void MainWindow::on_UpdateInfoButton_clicked()
{
    QString nameEdit, phoneEdit, passEdit;
    QSqlQuery qr;
    phoneEdit = "";
    qr.exec("update tanim set name = '"+nameEdit+"', phone = '"+phoneEdit+"', pass = '"+passEdit+"' where email = '"+em+"'");

    nameEdit = ui -> EnterNewName -> text();
    phoneEdit = ui -> NewPhone -> text();
    passEdit = ui -> NewPass -> text();
    if(nameEdit == "" || phoneEdit == "" || passEdit == "")
    {
            QMessageBox::warning(nullptr, "Error!", "Fill all the blanks!");
            return;
    }

    qr.exec("SELECT * FROM tanim WHERE  phone = '"+phoneEdit+"'");
    if(qr.next())
    {
            QMessageBox::warning(nullptr, "Error!", "Already Registered Number!");
            return;
    }
    if(phoneEdit.size() != 11)
    {
            QMessageBox::warning(nullptr, "Error!", "Invalid Number!");
            return;
    }
    qr.exec("update tanim set name = '"+nameEdit+"', phone = '"+phoneEdit+"', pass = '"+passEdit+"' where email = '"+em+"'");
    QMessageBox msgBox;
    msgBox.setText("Update Your Info!");
    msgBox.exec();
}


void MainWindow::on_ChangeInfoButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(14);
}


void MainWindow::on_BackButton_6_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}

