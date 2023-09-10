#include "ant-lib/mr25h.h"

//============================================================================================
//  Public
//============================================================================================

int Mr25h::init(ISpi* i_spi) {
    EHAS_INIT_DEVICE("Mr25h");
    EHAS_INIT_PACK(MR25H_ERR, NullPtr,      EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiWrite,     EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiRead,      EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiSelect,    EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, SpiDesel,     EHAS_ERROR);
    EHAS_INIT_PACK(MR25H_ERR, OutOfRange,   EHAS_ERROR);

    if (!i_spi) {
        EHAS_RETURN(MR25H_ERR_NullPtr);
    }

    this->spi = i_spi;

    this->write_cmd(MR25H_CMD_WREN);

    EHAS_RETURN_INIT_OK();
}

int Mr25h::write(uint32_t address, const uint8_t* data, uint32_t size) {
    if (address > MR25H_SIZE || size > MR25H_SIZE || address + size > MR25H_SIZE) {
        EHAS_RETURN(MR25H_ERR_OutOfRange);
    }
    uint8_t write_data[MR25H_ADDR_SIZE + sizeof(uint8_t)] = {0};
    write_data[0] = MR25H_CMD_WRITE;
    this->serialize_address(address, &write_data[1]);

    if (this->spi->select()) {
        EHAS_RETURN(MR25H_ERR_SpiSelect);
    }
    if (this->spi->write(write_data, sizeof(write_data)) != sizeof(write_data)) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->write(data, size) != size) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->deselect()) {
        EHAS_RETURN(MR25H_ERR_SpiDesel);
    }

    return size;
}

int Mr25h::read(uint32_t address, uint8_t* buffer, uint32_t lenght) {
    if (address > MR25H_SIZE || lenght > MR25H_SIZE || address + lenght > MR25H_SIZE) {
        EHAS_RETURN(MR25H_ERR_OutOfRange);
    }
    uint8_t write_data[MR25H_ADDR_SIZE + sizeof(uint8_t)] = {0};
    write_data[0] = MR25H_CMD_READ;
    this->serialize_address(address, &write_data[1]);

    if (this->spi->select()) {
        EHAS_RETURN(MR25H_ERR_SpiSelect);
    }
    if (this->spi->write(write_data, sizeof(write_data)) != sizeof(write_data)) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->read(buffer, lenght) != lenght) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->deselect()) {
        EHAS_RETURN(MR25H_ERR_SpiDesel);
    }

    return lenght;
}

//============================================================================================
//  Private
//============================================================================================

int Mr25h::set_lock_mode(mr25h_lock_t lock_mode) {
    return -1;
}

int Mr25h::read_status_reg(mr25h_reg_status_bf_t* status_bf) {
    if (!status_bf) {
        EHAS_RETURN(MR25H_ERR_NullPtr);
    }

    mr25h_reg_status_t reg = { 0 };

    for (int i = 0; i < 2; i++) {
        if (this->spi->select()) {
            EHAS_RETURN(MR25H_ERR_SpiSelect);
        }
        if (this->spi->write(&MR25H_CMD_RDSR, sizeof(uint8_t)) != sizeof(uint8_t)) {
            EHAS_RETURN(MR25H_ERR_SpiWrite);
        }
        if (this->spi->read(&reg.word, sizeof(uint8_t)) != sizeof(uint8_t)) {
            EHAS_RETURN(MR25H_ERR_SpiRead);
        }
        if (this->spi->deselect()) {
            EHAS_RETURN(MR25H_ERR_SpiDesel);
        }
    }

    *status_bf = reg.bf;
    return 0;
}

int Mr25h::write_status_reg(mr25h_reg_status_bf_t status_bf) {
    mr25h_reg_status_t reg = { .bf = status_bf };

    uint8_t write_data[2] = {0};
    write_data[0] = MR25H_CMD_WRSR;
    write_data[1] = reg.word;

    if (this->spi->select()) {
        EHAS_RETURN(MR25H_ERR_SpiSelect);
    }
    if (this->spi->write(write_data, sizeof(write_data)) != sizeof(write_data)) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->deselect()) {
        EHAS_RETURN(MR25H_ERR_SpiDesel);
    }

    return 0;
}

int Mr25h::write_cmd(uint8_t cmd) {
    if (this->spi->select()) {
        EHAS_RETURN(MR25H_ERR_SpiSelect);
    }
    if (this->spi->write(&cmd, sizeof(cmd)) != sizeof(cmd)) {
        EHAS_RETURN(MR25H_ERR_SpiWrite);
    }
    if (this->spi->deselect()) {
        EHAS_RETURN(MR25H_ERR_SpiDesel);
    }

    return 0;
}

void Mr25h::serialize_address(uint32_t addr, uint8_t* array) {
    array[0] = (addr & 0xFF0000) >> 16;
    array[1] = (addr & 0x00FF00) >> 8;
    array[2] = (addr & 0x0000FF) >> 0;
}


