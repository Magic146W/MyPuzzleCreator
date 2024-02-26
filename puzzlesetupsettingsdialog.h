#ifndef PUZZLESETUPSETTINGSDIALOG_H
#define PUZZLESETUPSETTINGSDIALOG_H

#include <QDialog>
#include <QSize>

namespace Ui {
class PuzzleSetUpSettingsDialog;
}

class PuzzleSetUpSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PuzzleSetUpSettingsDialog(const QVector<int>& imageSize, QWidget *parent = nullptr);
    ~PuzzleSetUpSettingsDialog();

private slots:
    void on_pushButton_Cancel_clicked();
    void on_pushButton_Accept_clicked();

    void onShapeNumberRowChanged(int index);
    void onShapeNumberColumnChanged(int index);

    void on_pushButton_columnMatch_clicked();

    void on_pushButton_row_clicked();

private:
    Ui::PuzzleSetUpSettingsDialog *ui;
    QVector<int> imageSize;
    QSize calculateShapeSize() const;
    void setUpComboBoxes(int maxRows, int maxColumns);

    int maxRows;
    int maxColumns;

signals:
    void acceptPuzzleDimensions(int rows, int columns);
};

#endif // PUZZLESETUPSETTINGSDIALOG_H
