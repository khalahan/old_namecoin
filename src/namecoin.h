
class CNameIndex
{
public:
    CDiskTxPos txPos;
    unsigned int nHeight;
    std::vector<unsigned char> vValue;

    CNameIndex()
    {
    }

    CNameIndex(CDiskTxPos txPosIn, unsigned int nHeightIn, std::vector<unsigned char> vValueIn)
    {
        txPos = txPosIn;
        nHeight = nHeightIn;
        vValue = vValueIn;
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(txPos);
        READWRITE(nHeight);
        READWRITE(vValue);
    )
};


/*
class CWalletDB : public CDB
{
public:
    CWalletDB(std::string strFilename, const char* pszMode="r+") : CDB(strFilename.c_str(), pszMode)
    {
    }
private:
    CWalletDB(const CWalletDB&);
    void operator=(const CWalletDB&);
public:
    bool WriteName(const std::string& strAddress, const std::string& strName);

    bool EraseName(const std::string& strAddress);

    bool WriteTx(uint256 hash, const CWalletTx& wtx)
    {
        nWalletDBUpdated++;
        return Write(std::make_pair(std::string("tx"), hash), wtx);
    }

    bool EraseTx(uint256 hash)
    {
        nWalletDBUpdated++;
        return Erase(std::make_pair(std::string("tx"), hash));
    }
*/

class CNameDB : public CDB
{
protected:
    bool fHaveParent;
public:
    CNameDB(const char* pszMode="r+") : CDB("nameindexfull.dat", pszMode) {
        fHaveParent = false;
    }

    CNameDB(const char* pszMode, CDB& parent) : CDB("nameindexfull.dat", pszMode) {
        //vTxn.push_back(parent.GetTxn());
        activeTxn = parent.activeTxn;
        fHaveParent = true;
    }

    ~CNameDB()
    {
        if (fHaveParent)
            //vTxn.erase(vTxn.begin());
            activeTxn = NULL;
    }

    //bool WriteName(std::vector<unsigned char>& name, std::vector<CDiskTxPos> vtxPos)
    bool WriteName(std::vector<unsigned char>& name, std::vector<CNameIndex>& vtxPos)
    {
        return Write(make_pair(std::string("namei"), name), vtxPos);
    }

    //bool ReadName(std::vector<unsigned char>& name, std::vector<CDiskTxPos>& vtxPos)
    bool ReadName(std::vector<unsigned char>& name, std::vector<CNameIndex>& vtxPos)
    {
        return Read(make_pair(std::string("namei"), name), vtxPos);
    }

    bool ExistsName(std::vector<unsigned char>& name)
    {
        return Exists(make_pair(std::string("namei"), name));
    }

    bool EraseName(std::vector<unsigned char>& name)
    {
        return Erase(make_pair(std::string("namei"), name));
    }

    bool ScanNames(
            const std::vector<unsigned char>& vchName,
            int nMax,
            std::vector<std::pair<std::vector<unsigned char>, CNameIndex> >& nameScan);
            //std::vector<std::pair<std::vector<unsigned char>, CDiskTxPos> >& nameScan);

    bool test();

    bool ReconstructNameIndex();
};

