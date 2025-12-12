/*
    ============================================================
    INCLUDE: Sistem Kendaraan Roleplay - Roleplay Vehicle System
    AUTHOR: Raihan Purnawadi
    ============================================================
    Script ini dibuat oleh Raihan Purnawadi untuk kemudahan pengelolaan kendaraan di server roleplay SA:MP.
    This script was created by Raihan Purnawadi for easy vehicle management in SA:MP roleplay servers.
    ------------------------------------------------------------
    Silakan gunakan & share, jangan hapus kredit pembuat!
    Feel free to use & share, but please do not remove the author's credit!
    ============================================================

    FITUR UTAMA / MAIN FEATURES:
      - Status mesin (engine on/off)
      - Sistem bahan bakar (fuel system)
      - Sistem kunci kendaraan (lock/unlock)
      - Bagasi kendaraan (vehicle trunk/inventory)
      - Sabuk pengaman (seatbelt)
      - Kap mesin & bagasi (hood & trunk open/close)
      - Odometer (kilometer/mileage)
      - Pemilik & plat nomor (owner & license plate)
      - Alarm kendaraan (car alarm)
      - Spawn & parkir kendaraan (spawn & parking point)
      - Penghancuran & respawn (destroy & respawn)
      - Anti hotwire (butuh kunci/skill membobol)
      - Simple key/command structure, plug and play!
*/

#if !defined MAX_VEHICLES
    #define MAX_VEHICLES 2000
#endif
#if !defined MAX_VEHICLE_TRUNK
    #define MAX_VEHICLE_TRUNK 10   // Maksimal item di bagasi (vehicle trunk)
#endif
#if !defined MAX_VEHICLE_PLATE
    #define MAX_VEHICLE_PLATE 16   // Maksimal karakter plat nomor/Max license plate chars
#endif

// -------- VARIABEL GLOBAL / GLOBAL VARIABLES --------

// Status mesin kendaraan (engine status): 0 = mati/off, 1 = nyala/on
new VehicleEngineStatus[MAX_VEHICLES];

// Status kunci kendaraan (lock status): 0 = unlocked, 1 = locked
new VehicleLockStatus[MAX_VEHICLES];

// Jumlah bahan bakar (fuel): float, 0.0 = empty
new Float:VehicleFuel[MAX_VEHICLES];

// Bagasi kendaraan berisi item ID (contoh ID bebas, silakan disesuaikan dengan sistem inventory server)
// Vehicle trunk holds item IDs
new VehicleTrunk[MAX_VEHICLES][MAX_VEHICLE_TRUNK];

// Status bagasi terbuka/tutup (0=closed, 1=open)
// Trunk open/close status
new VehicleTrunkOpen[MAX_VEHICLES];

// Status kap mesin terbuka/tutup (0=closed, 1=open)
// Hood open/close status
new VehicleHoodOpen[MAX_VEHICLES];

// Status sabuk pengaman per player (0=tidak/NO, 1=ya/YES); (bisa dihubungkan juga dengan OnPlayerStateChange)
// Player seatbelt status (support per player per vehicle, here simple global per vehicle)
new VehicleSeatbelt[MAX_VEHICLES];

// Nama pemilik kendaraan (Owner name)
new VehicleOwner[MAX_VEHICLES][24];

// Plat nomor kendaraan (License plate)
new VehiclePlate[MAX_VEHICLES][MAX_VEHICLE_PLATE];

// Status alarm kendaraan (car alarm): 0=off, 1=on
new VehicleAlarmActivated[MAX_VEHICLES];

// Odometer kendaraan (kilometer/mileage)
new Float:VehicleOdometer[MAX_VEHICLES];

// Titik parkir/spawn (vehicle parking/spawn point)
new Float:VehicleParkingX[MAX_VEHICLES];
new Float:VehicleParkingY[MAX_VEHICLES];
new Float:VehicleParkingZ[MAX_VEHICLES];
new Float:VehicleParkingA[MAX_VEHICLES];

// --------- FUNGSI-FUNGSI UTAMA / MAIN FUNCTIONS ------------

// ================= ENGINE FUNCTIONS ===================
stock IsVehicleEngineOn(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return -1;
    return VehicleEngineStatus[vehicleid];
}
stock SetVehicleEngineStatus(vehicleid, status)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleEngineStatus[vehicleid] = (status ? 1 : 0);
    return 1;
}

// ================= LOCK & HOTWIRE =====================
stock IsVehicleLocked(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return -1;
    return VehicleLockStatus[vehicleid];
}
stock SetVehicleLockStatus(vehicleid, status)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleLockStatus[vehicleid] = (status ? 1 : 0);
    return 1;
}
// Coba membobol/menghidupkan kendaraan tanpa kunci (hotwire attempt, success = 1, fail = 0)
// Bisa diintegrasikan dengan sistem skill / chance roll
stock TryVehicleHotwire(vehicleid, playerid)
{
    // Simple: 30% chance success
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    if(!IsVehicleLocked(vehicleid))
        return 1; // unlocked, no need hotwire
    if(random(100) < 30) // 30% chance
    {
        SetVehicleEngineStatus(vehicleid, 1);
        SetVehicleLockStatus(vehicleid, 0);
        // Kirim/Cetak log: (optional, customize your own print/message)
        // SendClientMessage(playerid, -1, "Hotwire sukses! (Hotwire success!)");
        return 1;
    }
    // SendClientMessage(playerid, -1, "Hotwire gagal! (Hotwire failed!)");
    return 0;
}

// ================ FUEL SYSTEM ==================
stock Float:GetVehicleFuel(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0.0;
    return VehicleFuel[vehicleid];
}
stock SetVehicleFuel(vehicleid, Float:amount)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleFuel[vehicleid] = (amount > 0.0 ? amount : 0.0);
    return 1;
}
stock VehicleFuelDecrease(vehicleid, Float:loss)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    if(loss <= 0.0) return 1;
    VehicleFuel[vehicleid] -= loss;
    if(VehicleFuel[vehicleid] < 0.0) VehicleFuel[vehicleid] = 0.0;
    return 1;
}

// ============== TRUNK/INVENTORY ================
stock TrunkAddItem(vehicleid, itemid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    for(new i=0; i<MAX_VEHICLE_TRUNK; i++)
    {
        if(VehicleTrunk[vehicleid][i] == 0)
        {
            VehicleTrunk[vehicleid][i] = itemid;
            return 1;
        }
    }
    return 0; // Penuh/full
}
stock TrunkRemoveItem(vehicleid, itemid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    for(new i=0; i<MAX_VEHICLE_TRUNK; i++)
    {
        if(VehicleTrunk[vehicleid][i] == itemid)
        {
            VehicleTrunk[vehicleid][i] = 0;
            return 1;
        }
    }
    return 0; // Tidak ada/not found
}
stock TrunkHasItem(vehicleid, itemid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    for(new i=0; i<MAX_VEHICLE_TRUNK; i++)
    {
        if(VehicleTrunk[vehicleid][i] == itemid) return 1;
    }
    return 0;
}
stock TrunkCountItems(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    new count = 0;
    for(new i=0; i<MAX_VEHICLE_TRUNK; i++) if(VehicleTrunk[vehicleid][i] != 0) count++;
    return count;
}

// ============ HOOD & TRUNK OPEN ===============
stock SetTrunkOpen(vehicleid, status)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleTrunkOpen[vehicleid] = (status ? 1 : 0);
    return 1;
}
stock SetHoodOpen(vehicleid, status)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleHoodOpen[vehicleid] = (status ? 1 : 0);
    return 1;
}
stock IsTrunkOpen(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return -1;
    return VehicleTrunkOpen[vehicleid];
}
stock IsHoodOpen(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return -1;
    return VehicleHoodOpen[vehicleid];
}

// ============== SEATBELT ======================
stock SetSeatbelt(vehicleid, status)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleSeatbelt[vehicleid] = (status ? 1 : 0);
    return 1;
}
stock HasSeatbelt(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    return VehicleSeatbelt[vehicleid];
}

// ============== ODOMETER ======================
stock Float:GetVehicleOdometer(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0.0;
    return VehicleOdometer[vehicleid];
}
stock AddVehicleOdometer(vehicleid, Float:add)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleOdometer[vehicleid] += (add > 0.0 ? add : 0.0);
    return 1;
}
stock ResetVehicleOdometer(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleOdometer[vehicleid] = 0.0;
    return 1;
}

// ============= OWNER & LICENSE PLATE ==========
stock SetVehicleOwner(vehicleid, const owner[])
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    format(VehicleOwner[vehicleid], 24, "%s", owner);
    return 1;
}
stock GetVehicleOwner(vehicleid, dest[], len)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    format(dest, len, "%s", VehicleOwner[vehicleid]);
    return 1;
}
stock SetVehiclePlate(vehicleid, const plate[])
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    format(VehiclePlate[vehicleid], MAX_VEHICLE_PLATE, "%s", plate);
    return 1;
}
stock GetVehiclePlate(vehicleid, dest[], len)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    format(dest, len, "%s", VehiclePlate[vehicleid]);
    return 1;
}

// =============== ALARM ========================
stock ActivateVehicleAlarm(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleAlarmActivated[vehicleid] = 1;
    // Add sound/notifications here as needed!
    return 1;
}
stock DeactivateVehicleAlarm(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleAlarmActivated[vehicleid] = 0;
    return 1;
}
stock IsVehicleAlarmOn(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    return VehicleAlarmActivated[vehicleid];
}

// =========== SPAWN & PARKING ==================
stock SetVehicleParking(vehicleid, Float:x, Float:y, Float:z, Float:a)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    VehicleParkingX[vehicleid] = x;
    VehicleParkingY[vehicleid] = y;
    VehicleParkingZ[vehicleid] = z;
    VehicleParkingA[vehicleid] = a;
    return 1;
}
stock GetVehicleParking(vehicleid, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    x = VehicleParkingX[vehicleid];
    y = VehicleParkingY[vehicleid];
    z = VehicleParkingZ[vehicleid];
    a = VehicleParkingA[vehicleid];
    return 1;
}

// Buat respawn vehicle ke parking
stock ParkingRespawnVehicle(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    SetVehiclePos(vehicleid, VehicleParkingX[vehicleid], VehicleParkingY[vehicleid], VehicleParkingZ[vehicleid]);
    SetVehicleZAngle(vehicleid, VehicleParkingA[vehicleid]);
    return 1;
}

// Destroy/Remove Vehicle, mark as destroyed
stock DestroyVehicleRoleplay(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES) return 0;
    DestroyVehicle(vehicleid);
    VehicleEngineStatus[vehicleid] = 0;
    VehicleLockStatus[vehicleid] = 0;
    VehicleFuel[vehicleid] = 0.0;
    VehicleOdometer[vehicleid] = 0.0;
    VehicleAlarmActivated[vehicleid] = 0;
    VehicleTrunkOpen[vehicleid] = 0;
    VehicleHoodOpen[vehicleid] = 0;
    // etc...
    return 1;
}

// ================ RESET ALL ===================
stock ResetAllVehicleStatus()
{
    for(new i = 0; i < MAX_VEHICLES; ++i)
    {
        VehicleEngineStatus[i] = 0;
        VehicleLockStatus[i] = 0;
        VehicleFuel[i] = 100.0;
        VehicleOdometer[i] = 0.0;
        VehicleAlarmActivated[i] = 0;
        VehicleTrunkOpen[i] = 0;
        VehicleHoodOpen[i] = 0;
        VehicleSeatbelt[i] = 0;
        for(new j=0;j<MAX_VEHICLE_TRUNK;j++) VehicleTrunk[i][j]=0;
        format(VehiclePlate[i],MAX_VEHICLE_PLATE,"UNKNOWN");
        format(VehicleOwner[i],24,"NONE");
        VehicleParkingX[i]=VehicleParkingY[i]=VehicleParkingZ[i]=VehicleParkingA[i]=0.0;
    }
    return 1;
}


/*
    =============================================================
    CONTOH PENGGUNAAN / USAGE EXAMPLES:
    =================
    // Nyala/matikan mesin (engine on/off)
    SetVehicleEngineStatus(vehicleid, 1); // Nyala / ON
    SetVehicleEngineStatus(vehicleid, 0); // Mati / OFF
    
    // Kunci/Buka kendaraan (lock/unlock)
    SetVehicleLockStatus(vehicleid, 1); // Lock
    SetVehicleLockStatus(vehicleid, 0); // Unlock

    // Tambah bahan bakar (add fuel)
    SetVehicleFuel(vehicleid, 50.0);

    // Trunk add/remove item:
    TrunkAddItem(vehicleid, 101); // 101 = item id

    // Seatbelt:
    SetSeatbelt(vehicleid, 1); // Pakai seatbelt / Use seatbelt

    // Buka tutup bagasi/kap mesin (open/close trunk/hood)
    SetTrunkOpen(vehicleid, 1); // Buka/Open
    SetHoodOpen(vehicleid, 1); // Buka/Open

    // Odometer:
    AddVehicleOdometer(vehicleid, 1.5); // +1.5 km

    // Alarm:
    ActivateVehicleAlarm(vehicleid);
    DeactivateVehicleAlarm(vehicleid);

    // Parkir/Spawn Reset:
    SetVehicleParking(vehicleid, x, y, z, a);
    ParkingRespawnVehicle(vehicleid);

    // Destroy:
    DestroyVehicleRoleplay(vehicleid);

    =============================================================
    Tinggal plug and play untuk server roleplaymu!
    Just plug and play for your roleplay server!

    Kontribusi & kredit WAJIB dicantumkan: Raihan Purnawadi
    Contribution & credit REQUIRED: Raihan Purnawadi
    =============================================================
*/

