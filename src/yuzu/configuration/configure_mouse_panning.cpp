// SPDX-FileCopyrightText: 2023 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QCloseEvent>

#include "common/settings.h"
#include "ui_configure_mouse_panning.h"
#include "yuzu/configuration/configure_mouse_panning.h"

ConfigureMousePanning::ConfigureMousePanning(QWidget* parent,
                                             InputCommon::InputSubsystem* input_subsystem_,
                                             float right_stick_deadzone, float right_stick_range)
    : QDialog(parent), input_subsystem{input_subsystem_},
      ui(std::make_unique<Ui::ConfigureMousePanning>()) {
    ui->setupUi(this);
    SetConfiguration(right_stick_deadzone, right_stick_range);
    ConnectEvents();
}

ConfigureMousePanning::~ConfigureMousePanning() = default;

void ConfigureMousePanning::closeEvent(QCloseEvent* event) {
    event->accept();
}

void ConfigureMousePanning::SetConfiguration(float right_stick_deadzone, float right_stick_range) {
    ui->enable->setChecked(Settings::values.mouse_panning.GetValue());
    ui->x_sensitivity->setValue(Settings::values.mouse_panning_x_sensitivity.GetValue());
    ui->y_sensitivity->setValue(Settings::values.mouse_panning_y_sensitivity.GetValue());
    ui->deadzone_x_counterweight->setValue(
        Settings::values.mouse_panning_deadzone_x_counterweight.GetValue());
    ui->deadzone_y_counterweight->setValue(
        Settings::values.mouse_panning_deadzone_y_counterweight.GetValue());
    ui->decay_strength->setValue(Settings::values.mouse_panning_decay_strength.GetValue());
    ui->min_decay->setValue(Settings::values.mouse_panning_min_decay.GetValue());

    if (right_stick_deadzone > 0.0f || right_stick_range != 1.0f) {
        ui->warning_label->setText(QString::fromStdString(
            "Mouse panning works better with a deadzone of 0% and a range of 100%.\n"
            "Current values are " +
            std::to_string(static_cast<int>(right_stick_deadzone * 100.0f)) + "% and " +
            std::to_string(static_cast<int>(right_stick_range * 100.0f)) + "% respectively."));
    } else {
        ui->warning_label->hide();
    }
}

void ConfigureMousePanning::SetDefaultConfiguration() {
    ui->x_sensitivity->setValue(Settings::values.mouse_panning_x_sensitivity.GetDefault());
    ui->y_sensitivity->setValue(Settings::values.mouse_panning_y_sensitivity.GetDefault());
    ui->deadzone_x_counterweight->setValue(
        Settings::values.mouse_panning_deadzone_x_counterweight.GetDefault());
    ui->deadzone_y_counterweight->setValue(
        Settings::values.mouse_panning_deadzone_y_counterweight.GetDefault());
    ui->decay_strength->setValue(Settings::values.mouse_panning_decay_strength.GetDefault());
    ui->min_decay->setValue(Settings::values.mouse_panning_min_decay.GetDefault());
}

void ConfigureMousePanning::ConnectEvents() {
    connect(ui->default_button, &QPushButton::clicked, this,
            &ConfigureMousePanning::SetDefaultConfiguration);
    connect(ui->button_box, &QDialogButtonBox::accepted, this,
            &ConfigureMousePanning::ApplyConfiguration);
    connect(ui->button_box, &QDialogButtonBox::rejected, this, [this] { reject(); });
}

void ConfigureMousePanning::ApplyConfiguration() {
    Settings::values.mouse_panning = ui->enable->isChecked();
    Settings::values.mouse_panning_x_sensitivity = static_cast<float>(ui->x_sensitivity->value());
    Settings::values.mouse_panning_y_sensitivity = static_cast<float>(ui->y_sensitivity->value());
    Settings::values.mouse_panning_deadzone_x_counterweight =
        static_cast<float>(ui->deadzone_x_counterweight->value());
    Settings::values.mouse_panning_deadzone_y_counterweight =
        static_cast<float>(ui->deadzone_y_counterweight->value());
    Settings::values.mouse_panning_decay_strength = static_cast<float>(ui->decay_strength->value());
    Settings::values.mouse_panning_min_decay = static_cast<float>(ui->min_decay->value());

    accept();
}