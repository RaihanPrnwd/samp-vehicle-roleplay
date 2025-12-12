# vehicle-RP.inc

**Versi Roleplay Vehicle System untuk SA-MP (PAWN)**  
oleh Raihan Purnawadi

---

Sistem include SA-MP untuk mempermudah pembuatan fitur roleplay kendaraan pada servermu.  
Cukup plug and play, mudah diintegrasikan ke gamemode existing!

## ✨ Fitur Utama
- Status mesin kendaraan (engine on/off)
- Sistem bahan bakar kendaraan (fuel system)
- Sistem kunci kendaraan (lock/unlock)
- Bagasi/Inventory kendaraan (vehicle trunk)
- Sabuk pengaman (seatbelt)
- Kap mesin & bagasi open/close
- Odometer (kilometer/kilometeran)
- Pemilik & plat nomor
- Alarm dan status alarm
- Titik parkir & respawn
- Fitur anti hotwire (butuh kunci/membobol)
- Strukturnya sederhana, stock function friendly

---

## ⏳ Cara Pakai Singkat

1. **Copy file `vehicle-RP.inc` ke folder `include/` pada server-mu**
2. **Di script utama (misal gamemodes/mygm.pwn) tambahkan:**
   ```pawn
   #include <vehicle-RP>
   ```
3. **Gunakan fungsi-fungsi berikut di script-mu:**
   - Nyalakan mesin:
     ```pawn
     SetVehicleEngineStatus(vehicleid, 1);    // ON
     SetVehicleEngineStatus(vehicleid, 0);    // OFF
     ```
   - Kunci/Buka kendaraan:
     ```pawn
     SetVehicleLockStatus(vehicleid, 1);      // Lock
     SetVehicleLockStatus(vehicleid, 0);      // Unlock
     ```
   - Tambah bahan bakar:
     ```pawn
     SetVehicleFuel(vehicleid, 50.0);
     ```
   - Fungsi lain: lihat daftar lengkap di bawah! 👇

---

## 🏷️ Daftar Fungsi Penting

| Fungsi                                 | Keterangan / Return |
| --------------------------------------- | ------------------- |
| `SetVehicleEngineStatus(vehicleid, st)` | Set nyala/mati mesin (1/0) |
| `IsVehicleEngineOn(vehicleid)`          | Status mesin (1/0/-1)      |
| `SetVehicleLockStatus(vehicleid, st)`   | Lock/Unlock kendaraan      |
| `IsVehicleLocked(vehicleid)`            | Status kunci (1/0/-1)      |
| `SetVehicleFuel(vehicleid, Float:amt)`  | Atur bahan bakar           |
| `Float:GetVehicleFuel(vehicleid)`       | Ambil jumlah bahan bakar   |
| `TrunkAddItem(vehicleid, itemid)`       | Tambah item ke trunk       |
| `TrunkRemoveItem(vehicleid, itemid)`    | Hapus item dari trunk      |
| `TrunkHasItem(vehicleid, itemid)`       | Cek item ada/tidak         |
| `TrunkCountItems(vehicleid)`            | Hitung jumlah item         |
| `SetTrunkOpen(vehicleid, status)`       | Buka/tutup bagasi          |
| `SetHoodOpen(vehicleid, status)`        | Buka/tutup kap mesin       |
| `SetSeatbelt(vehicleid, status)`        | Set status seatbelt        |
| `HasSeatbelt(vehicleid)`                | Cek seatbelt (1/0)         |
| `SetVehicleOwner(vehicleid, owner[])`   | Atur nama pemilik          |
| `GetVehicleOwner(vehicleid, dest[],len)`| Ambil nama pemilik         |
| `SetVehiclePlate(vehicleid, plate[])`   | Atur plat nomor            |
| `GetVehiclePlate(vehicleid, dest[],len)`| Ambil plat kendaraan       |
| `ActivateVehicleAlarm(vehicleid)`       | Aktifkan alarm             |
| `DeactivateVehicleAlarm(vehicleid)`     | Matikan alarm              |
| `IsVehicleAlarmOn(vehicleid)`           | Status alarm (1/0)         |
| `SetVehicleParking(vehicleid,x,y,z,a)`  | Set posisi parkir          |
| `ParkingRespawnVehicle(vehicleid)`      | Respawn ke titik parkir    |
| `DestroyVehicleRoleplay(vehicleid)`     | Hapus & reset data kendaraan|
| `ResetAllVehicleStatus()`               | Reset SEMUA data kendaraan |

_Catat: Semua validasi index vehicle telah built-in di fungsi (tidak perlu cek negatif/overflow sendiri)._

---

## 💡 Contoh Penggunaan

```pawn
#include <a_samp>
#include <vehicle-RP>

public OnPlayerCommandText(playerid, cmdtext[])
{
    if(strcmp(cmdtext, "/engineon", true)==0)
    {
        if(IsPlayerInAnyVehicle(playerid))
        {
            new vid = GetPlayerVehicleID(playerid);
            SetVehicleEngineStatus(vid, 1);
            SendClientMessage(playerid, -1, "Mesin kendaraan dinyalakan!");
        }
        return 1;
    }

    if(strcmp(cmdtext, "/lockcar", true)==0)
    {
        if(IsPlayerInAnyVehicle(playerid))
        {
            new vid = GetPlayerVehicleID(playerid);
            SetVehicleLockStatus(vid, 1);
            SendClientMessage(playerid, -1, "Kendaraan dikunci!");
        }
        return 1;
    }
    return 0;
}
```

---

## 📚 Integrasi Fuel System, Odometer dsb

Kombinasikan dengan event SA-MP seperti OnVehicleUpdate, OnPlayerStateChange, dsb untuk update bahan bakar/odometer secara otomatis.

---

## ⚠️ (PERHATIAN) / Catatan Dev

- Maksimal kendaraan default: 2000 (`MAX_VEHICLES`).
- Maksimal item bagasi: 10 (ubah `MAX_VEHICLE_TRUNK` jika perlu).
- Owner/plat hardcoded di array, pastikan cukup besar jika ingin dinamis.

---

## 👨‍💻 Kredit & Lisensi

Script ini dibuat oleh **Raihan Purnawadi**  
Silakan digunakan, disebar, dan dimodifikasi selama mencantumkan kredit pembuat.

---

_Semoga memudahkan roleplay di servermu!_ 🚗🚖🚙  
Untuk kontribusi atau masalah, hubungi GitHub/repo/akun pembuat.

---
