//
// Created by lv on 5/5/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PHOTOPICKERSCREEN_H
#define LAB6_LOGIN_REGISTRATION_PHOTOPICKERSCREEN_H
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <vector>
#include "core/model/User.h"
#include "core/model/Photo.h"

class PhotoThumbnailWidget;

class PhotoPickerScreen : public QMainWindow {
    Q_OBJECT

public:
    explicit PhotoPickerScreen(QWidget* parent = nullptr);
    ~PhotoPickerScreen() = default;

    void setUser(const Core::Entities::User& user);

    signals:
        void backToProfile();
    void avatarUpdated();

private slots:
    void onUploadPhoto();
    void onPhotoSelected(const Core::Entities::Photo& photo);
    void onDeletePhoto(const Core::Entities::Photo& photo);
    void onSetAsAvatar(const Core::Entities::Photo& photo);
    void onBackClicked();
    void onRefresh();

private:
    void setupUI();
    void loadPhotos();
    void showStatusMessage(const QString& message, bool isError = false);
    void clearSelection();
    void updatePhotoLimitWarning();

    // UI Components
    QLabel* m_titleLabel;
    QLabel* m_photoCountLabel;
    QPushButton* m_uploadBtn;
    QScrollArea* m_scrollArea;
    QWidget* m_photoGridWidget;
    QGridLayout* m_photoGridLayout;
    QLabel* m_statusLabel;
    QPushButton* m_refreshBtn;
    QPushButton* m_backBtn;

    // Data
    Core::Entities::User m_currentUser;
    std::vector<Core::Entities::Photo> m_userPhotos;
    std::vector<PhotoThumbnailWidget*> m_thumbnailWidgets;
    int m_selectedPhotoId;  // <-- ADD THIS LINE
};
#endif //LAB6_LOGIN_REGISTRATION_PHOTOPICKERSCREEN_H