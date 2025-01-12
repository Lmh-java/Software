from PyQt6.QtWidgets import QWidget
from pyqttoast import Toast, ToastPreset


def show_toast(
    parent: QWidget, title: str, text: str, timeout: int, preset: ToastPreset
) -> None:
    """Display a toast message with the given properties
    :param parent: parent widget to inject toast message component
    :param title: title of toast message
    :param text: text of toast message
    :param timeout: duration of the message (in ms)
    :param preset: preset of the message box
    """
    toast = Toast(parent)
    toast.setDuration(timeout)
    toast.setTitle(title)
    toast.setText(text)
    toast.applyPreset(preset)
    toast.setShowCloseButton(False)
    toast.setBorderRadius(10)
    toast.setShowDurationBar(False)
    toast.show()


def success_toast(parent: QWidget, text: str, timeout: int = 1000) -> None:
    """Display a success toast message
    :param parent: parent widget to inject toast message component
    :param text: text of toast message
    :param timeout: duration of the message (in ms)
    """
    show_toast(parent, "", text, timeout, ToastPreset.SUCCESS_DARK)


def failure_toast(parent: QWidget, text: str, timeout: int = 1000) -> None:
    """Display a failure toast message
    :param parent: parent widget to inject toast message component
    :param text: text of toast message
    :param timeout: duration of the message (in ms)
    """
    show_toast(parent, "", text, timeout, ToastPreset.ERROR_DARK)


def info_toast(parent: QWidget, text: str, timeout: int = 1000) -> None:
    """Display an information toast message
    :param parent: parent widget to inject toast message component
    :param text: text of toast message
    :param timeout: duration of the message (in ms)
    """
    show_toast(parent, "", text, timeout, ToastPreset.INFORMATION_DARK)


def warning_toast(parent: QWidget, text: str, timeout: int = 1000) -> None:
    """Display a warning toast message
    :param parent: parent widget to inject toast message component
    :param text: text of toast message
    :param timeout: duration of the message (in ms)
    """
    show_toast(parent, "", text, timeout, ToastPreset.WARNING_DARK)