
class QByteArray;
class QTextCodec;

bool GetDetectedCodecGoogle(const QByteArray& array, QTextCodec*& codec);
bool GetDetectedCodecICU(const QByteArray& array, QTextCodec*& codec);
