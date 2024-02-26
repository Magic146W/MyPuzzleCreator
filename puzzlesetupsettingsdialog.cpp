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

    maxRows = imageSize[0] / 57;
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
    qreal difference = 999;
    for (int i = 0; i < maxRows; ++i)
    {
        ui->comboBox_row->setCurrentIndex(i);
        calculateShapeSize();

        qreal columnValue = ui->label_ShapeSize_Columns->text().toFloat();
        qreal rowValue = ui->label_ShapeSize_Rows->text().toFloat();

        if(difference < qAbs(columnValue - rowValue))
        {
            if(i != 0)
            {
                ui->comboBox_row->setCurrentIndex(i-1);
                return;
            } else
            {
                ui->comboBox_row->setCurrentIndex(0);
            }
        } else
        {
            difference = qAbs(columnValue - rowValue);
        }
    }
}


void PuzzleSetUpSettingsDialog::on_pushButton_row_clicked()
{
    qreal difference = 999;
    for (int i = 0; i < maxColumns; ++i)
    {
        ui->comboBox_column->setCurrentIndex(i);
        calculateShapeSize();

        qreal columnValue = ui->label_ShapeSize_Columns->text().toFloat();
        qreal rowValue = ui->label_ShapeSize_Rows->text().toFloat();

        if(difference < qAbs(columnValue - rowValue))
        {
            if(i != 0)
            {
                ui->comboBox_column->setCurrentIndex(i-1);
                return;
            } else
            {
                ui->comboBox_column->setCurrentIndex(0);
            }
        } else
        {
            difference = qAbs(columnValue - rowValue);
        }
    }
}

