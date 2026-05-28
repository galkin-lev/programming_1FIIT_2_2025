//
// Created by lv on 5/5/26.
//

#ifndef LAB6_LOGIN_REGISTRATION_PHOTOPICKERTHUMBNAILWIDGET_H
#define LAB6_LOGIN_REGISTRATION_PHOTOPICKERTHUMBNAILWIDGET_H



#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "core/model/Photo.h"

class PhotoThumbnailWidget : public QWidget {
    Q_OBJECT

public:
    explicit PhotoThumbnailWidget(const Core::Entities::Photo& photo, QWidget* parent = nullptr);

    Core::Entities::Photo getPhoto() const { return m_photo; }
    void setSelected(bool selected);
    void setAsAvatar(bool isAvatar);

    signals:
        void clicked(const Core::Entities::Photo& photo);
    void deleteRequested(const Core::Entities::Photo& photo);
    void setAsAvatarRequested(const Core::Entities::Photo& photo);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void setupUI();
    void updateStyle();

    Core::Entities::Photo m_photo;
    QLabel* m_thumbnailLabel;
    QLabel* m_nameLabel;
    QLabel* m_sizeLabel;
    QPushButton* m_deleteBtn;
    QPushButton* m_avatarBtn;
    QWidget* m_selectionOverlay;
    bool m_isSelected;
    bool m_isHovered;
};

#endif //LAB6_LOGIN_REGISTRATION_PHOTOPICKERTHUMBNAILWIDGET_H