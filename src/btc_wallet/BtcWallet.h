#ifndef BTCWALLET_H
#define BTCWALLET_H

#include <string>
#include <vector>
#include <set>

struct BtcInput {
    std::string tx_hash;
    uint32_t tx_index;
    std::string scriptPubkey;
    uint64_t value;

    bool operator< (const BtcInput &second) const {
        return this->value < second.value;
    }

    BtcInput(const std::string &spendtxid, uint32_t spendoutnum, const std::string &scriptPubkey, uint64_t outBalance)
        : tx_hash(spendtxid)
        , tx_index(spendoutnum)
        , scriptPubkey(scriptPubkey)
        , value(outBalance)
    {}
    
    BtcInput(uint64_t outBalance)
        : value(outBalance)
    {}

    BtcInput() = default;

};

class BtcWallet {
public:

    static std::string getFullPath(const std::string &folder, const std::string &address);

    static std::pair<std::string, std::string> genPrivateKey(const std::string &folder, const std::string &password);

    BtcWallet(const std::string &folder, const std::string &address, const std::string &password);

    BtcWallet(const std::string &decryptedWif);

    // Используется только для тестов. Для работы использовать buildTransaction
    std::string genTransaction(const std::vector<BtcInput> &inputs, uint64_t transferAmount, uint64_t fee, const std::string &receiveAddress, bool isTestnet);

    static std::vector<BtcInput> reduceInputs(const std::vector<BtcInput> &inputs, const std::set<std::string> &usedTxs);

    std::pair<std::string, std::set<std::string>> buildTransaction(
        const std::vector<BtcInput> &utxos,
        size_t estimateComissionInSatoshi,
        const std::string &valueStr,
        const std::string &feesStr,
        const std::string &receiveAddress
    );

    static std::string calcHashNotWitness(const std::string &txHex);

    static std::vector<std::pair<std::string, std::string>> getAllWalletsInFolder(const std::string &folder);

    const std::string& getAddress() const;

    static void checkAddress(const std::string &address);

private:

    BtcWallet(const std::string &fileData, const std::string &password);

private:

    std::pair<std::string, std::set<std::string>> encode(
        bool allMoney, const int64_t &value, const int64_t &fees,
        const std::string &toAddress,
        const std::vector<BtcInput> &utxos
    );

    std::string wif;

    std::string address;
};

#endif // BTCWALLET_H
