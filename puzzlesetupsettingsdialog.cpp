#include "puzzlesetupsettingsdialog.h"
#include "ui_puzzlesetupsettingsdialog.h"
#include <QIntValidator>
#include <QVector>
#include <QButtonGroup>

PuzzleSetUpSettingsDialog::PuzzleSetUpSettingsDialog(const QVector<int> &imageSize, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PuzzleSetUpSettingsDialog)
    ,imageSize(imageSize)
{
    ui->setupUi(this);

    maxRows = imageSize[0] / 57; // approximate pixels for 1.5cm
    ui->label_rows->setText(ui->label_rows->text() + QString::number(maxRows));
    maxColumns = imageSize[1] / 57;
    ui->label_columns->setText(ui->label_columns->text() + QString::number(maxColumns));

    setUpComboBoxes(maxRows, maxColumns);

    calculateShapeSize();

    connect(ui->comboBox_row, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PuzzleSetUpSettingsDialog::onShapeNumberRowChanged);
    connect(ui->comboBox_column, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PuzzleSetUpSettingsDialog::onShapeNumberColumnChanged);
}

PuzzleSetUpSettingsDialog::~PuzzleSetUpSettingsDialog()
{
    delete ui;
}

void PuzzleSetUpSettingsDialog::onShapeNumberRowChanged(int index)
{
    Q_UNUSED(index);
    calculateShapeSize();
}

void PuzzleSetUpSettingsDialog::onShapeNumberColumnChanged(int index)
{
    Q_UNUSED(index);
    calculateShapeSize();
}

QSize PuzzleSetUpSettingsDialog::calculateShapeSize() const
{
    int rowCount = ui->comboBox_row->currentText().toInt();
    int columnCount = ui->comboBox_column->currentText().toInt();

    int imageHeight = imageSize[0];
    int imageWidth = imageSize[1];

    int shapeWidth = imageWidth / columnCount;
    int shapeHeight = imageHeight / rowCount;

    double cmPerPixel = 0.0264583333; // 1 pixel in cm
    double shapeWidthCm = shapeWidth * cmPerPixel;
    double shapeHeightCm = shapeHeight * cmPerPixel;

    ui->label_ShapeSize_Columns->setText(QString::number(shapeWidthCm, 'f', 2));

    ui->label_ShapeSize_Rows->setText(QString::number(shapeHeightCm, 'f', 2));

    return QSize(shapeWidth, shapeHeight);
}

void PuzzleSetUpSettingsDialog::setUpComboBoxes(int maxRows, int maxColumns)
{
    for (int i = 2; i <= maxRows; ++i)
    {
        ui->comboBox_row->addItem(QString::number(i));
    }

    for (int i = 2; i <= maxColumns; ++i)
    {
        ui->comboBox_column->addItem(QString::number(i));
    }
}

void PuzzleSetUpSettingsDialog::on_pushButton_Cancel_clicked()
{
    close();
}

void PuzzleSetUpSettingsDialog::on_pushButton_Accept_clicked()
{
    int shapeNumberRow = ui->comboBox_row->currentText().toInt();
    int shapeNumberColumn = ui->comboBox_column->currentText().toInt();

    emit acceptPuzzleDimensions(shapeNumberRow, shapeNumberColumn);

    close();
}

void PuzzleSetUpSettingsDialog::on_pushButton_columnMatch_clicked()
{
    int row = maxRows*ui->comboBox_column->currentText().toInt()/maxColumns; // get rows equivalent based on percentage calculations
    if(row>maxRows)
    {
        row = maxRows;
    }

    row -= 2; //get index
    if(row < 0)
    {
        row = 0;
    }

    ui->comboBox_row->setCurrentIndex(row);
}


void PuzzleSetUpSettingsDialog::on_pushButton_row_clicked()
{
    int column = maxColumns*ui->comboBox_row->currentText().toInt()/maxRows;
    if(column>maxColumns)
    {
        column = maxColumns;
    }

    column -= 2;
    if(column < 0)
    {
        column = 0;
    }

    ui->comboBox_column->setCurrentIndex(column);
}

