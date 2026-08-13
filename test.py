import win32com.client
dm = win32com.client.Dispatch('dm.dmsoft')
print(dm.findpic(0, 0, 2560, 1440, "mage.png", "00000", 0.8, 0))