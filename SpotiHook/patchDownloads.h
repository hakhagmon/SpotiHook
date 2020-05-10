// ==========================================================
// Project: XSpotify
// 
// Component: XSpotify.dll
//
// Purpose: Downloading and decrypting Spotify songs directly
//          from Spotify servers
//
// Initial author: Meik Wochnik
//
// Started: 28.12.2019
//
// *thanks to BAKADESU
// ==========================================================


#pragma once
//#define TAGLIB_STATIC
//#include <ogg/vorbis/vorbisfile.h>
//#include <tagutils.h>
#include "imports.h"
#include "hooking.h"
#include "hooks.h"
#include "utils.h"
#include "patchAds.h"
#include <openssl/evp.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "libcryptoMT.lib")
//#pragma comment(lib, "tag.lib")

DWORD AES_set_encrypt_key_back = AES_SET_ENCRYPT_KEY + 6;
DWORD GetFileID_back = GETFILEID + 9;
DWORD Signal_back = SIGNAL + 10;
bool check = false;

struct DecryptedSong_s
{
	int decryptedtext_len;
	unsigned char rawDecryptedData[50000000];
	unsigned char outputData[50000000];
	unsigned char songName_json[5000];
	std::string currentSong;
	std::string songName;
	std::string artist;
	std::string album;
	std::string coverart;
	std::string rawSongData;
} DecryptedSong;

struct EncryptedSong_s
{
	unsigned char encryptedData[50000000];
	const char* fileID;
	std::string rawSongData;
	std::string rawHostData;
	std::string parsedHost;
	std::string parsedParam;
	std::string decryptionKey;
} EncryptedSong;

struct InternetScopedHandle
{
	InternetScopedHandle(HINTERNET handle)
		: handle(handle) {  }

	HINTERNET handle;

	operator HINTERNET()
	{
		return handle;
	}

	~InternetScopedHandle()
	{
		assert(InternetCloseHandle(handle));
	}
};

//https://gist.github.com/guymac/1468279
/*class ImageFile : public TagLib::File
{
public:
	ImageFile(const wchar_t* file) : TagLib::File(file)
	{

	}

	TagLib::ByteVector data()
	{
		return readBlock(length());
	}


private:
	virtual TagLib::Tag* tag() const { return 0; }
	virtual TagLib::AudioProperties* audioProperties() const { return 0; }
	virtual bool save() { return false; }
}; */

std::string HttpRequest(std::string site, std::string param, std::string accessToken, bool isaccessToken)
{
	std::string authHeader;
	InternetScopedHandle hInternet = InternetOpenW(L"1337", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (isaccessToken == true)
	{
		authHeader = "Authorization: Bearer " + accessToken;
	}
	else
	{
		authHeader = "1337";
	}

	if (hInternet == NULL)
	{
		return "InternetOpenW failed(hInternet): " + std::to_string(GetLastError());
	}

	InternetScopedHandle hConnect = InternetConnectA(hInternet, site.c_str(), 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

	if (hConnect == NULL)
	{
		return "InternetConnectW failed(hConnect == NULL): " + std::to_string(GetLastError());
	}
	InternetScopedHandle hRequest = HttpOpenRequestA(hConnect, "GET", param.c_str(), NULL, NULL, NULL, INTERNET_FLAG_NO_AUTH, 0);

	if (hRequest == NULL)
	{
		return "HttpOpenRequestW failed(hRequest == NULL): " + std::to_string(GetLastError());
	}
	HttpAddRequestHeadersA(hRequest, authHeader.c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	BOOL bRequestSent = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);

	if (!bRequestSent)
	{
		return "!bRequestSent    HttpSendRequestW failed with error code " + std::to_string(GetLastError());
	}

	std::string strResponse;
	const int nBuffSize = 1024;
	char buff[nBuffSize];

	BOOL bKeepReading = true;
	DWORD dwBytesRead = -1;

	while (bKeepReading && dwBytesRead != 0)
	{
		bKeepReading = InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
		strResponse.append(buff, dwBytesRead);
	}
	return strResponse;
}

int decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext)
{
	EVP_CIPHER_CTX* ctx;

	int len;

	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		Utils::handleErrors();

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv))
		Utils::handleErrors();


	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		Utils::handleErrors();
	plaintext_len = len;


	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		Utils::handleErrors();
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

void __declspec(naked) AES_set_encrypt_key_stub(unsigned int* key, DWORD* userKey, int bits)
{
	__asm
	{
		push     ebp
		mov      ebp, esp
		mov      edx, [ebp + 8]
		push     AES_set_encrypt_key_back
		retn
	}
}

void AES_set_encrypt_key_hk(unsigned int* key, DWORD* userKey, int bits)
{
	unsigned char decrypt_key[16] = { 0 };
	char decrypt_key_str[33] = { 0 };

	if (bits == 128)
	{
		unsigned char _userKey[16] = { 0 };

		for (int i = 0; i < 16; i++)
		{
			_userKey[i] = *(char*)((DWORD)userKey + i);
		}

		if (memcmp(_userKey, decrypt_key, 16) != 0)
		{
			for (int i = 0; i < 16; i++)
			{
				sprintf(decrypt_key_str + i * 2, "%02X", _userKey[i]);
			}

			EncryptedSong.decryptionKey = decrypt_key_str;
		}
	}

	AES_set_encrypt_key_stub(key, userKey, bits);
}

std::string GetAccessToken()
{
	ShellExecute(NULL, "open", ".\\token.exe", NULL, NULL, SW_HIDE);
	Sleep(1000);

	std::ifstream ifs("token.txt");
	std::string accessToken((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	return accessToken;
}

std::string GetRawSongData()
{
	std::string SongID = EncryptedSong.fileID;
	EncryptedSong.rawHostData = HttpRequest("spclient.wg.spotify.com", "/storage-resolve/files/audio/interactive_prefetch/" + SongID + "?product=0", GetAccessToken(), true);
	EncryptedSong.parsedHost = (EncryptedSong.rawHostData.substr(EncryptedSong.rawHostData.find("https://") + 8)).erase(EncryptedSong.rawHostData.substr(EncryptedSong.rawHostData.find("https://") + 8).find("/audio/"));
	EncryptedSong.parsedParam = EncryptedSong.rawHostData.substr(EncryptedSong.rawHostData.find("/audio/")).erase(EncryptedSong.rawHostData.substr(EncryptedSong.rawHostData.find("/audio/")).find("="));
	EncryptedSong.rawSongData = HttpRequest(EncryptedSong.parsedHost, EncryptedSong.parsedParam, "", false);
	return EncryptedSong.rawSongData;
}

std::string DeleteOGGHeader(std::string rawSongData)
{
	return rawSongData.substr(rawSongData.find("ÿÿÿÿOggS") + 4);
}

void AddTags(std::wstring filename, std::wstring artist, std::wstring album, std::wstring coverarturl)
{
	wstring newname = L".\\Songs\\" + filename + L".ogg";
	wstring image = L".\\Songs\\" + filename + L".jpg";
	//URLDownloadToFile(0, coverarturl.c_str(), image.c_str(), 0, 0);

	/*TagLib::Ogg::Vorbis::File f(newname.c_str());
	TagLib::Ogg::XiphComment* tag = f.tag();
	TagLib::FLAC::Picture* picture = new TagLib::FLAC::Picture();
	ImageFile imageFile(image.c_str());
	TagLib::ByteVector imageData = imageFile.data();

	picture->setData(imageData);
	picture->setType((TagLib::FLAC::Picture::Type)  0x03);
	picture->setMimeType("image/jpeg");
	picture->setDescription("Front Cover");

	TagLib::ByteVector block = picture->render();
	tag->addField("METADATA_BLOCK_PICTURE", Utils::b64_encode((BYTE*)block.data(), block.size()), true);
	f.tag()->setArtist(artist);
	f.tag()->setAlbum(album);
	f.save(); */
}

void DownloadFileProcess()
{
	std::cout << "[Download started]" << std::endl;

	std::string IVKEY = Utils::hex2string("72E067FBDDCBCF77EBE8BC643F630D93");
	std::string KEY = Utils::hex2string(EncryptedSong.decryptionKey);

	memcpy(EncryptedSong.encryptedData, GetRawSongData().data(), GetRawSongData().size());
	DecryptedSong.decryptedtext_len = decrypt(EncryptedSong.encryptedData, GetRawSongData().length(), (unsigned char*)KEY.c_str(), (unsigned char*)IVKEY.c_str(), DecryptedSong.rawDecryptedData);
	DecryptedSong.rawSongData.assign((char*)&DecryptedSong.rawDecryptedData[0], sizeof(DecryptedSong.rawDecryptedData));
	memcpy(DecryptedSong.outputData, DeleteOGGHeader(DecryptedSong.rawSongData).data(), DeleteOGGHeader(DecryptedSong.rawSongData).size());

	if (!DecryptedSong.currentSong.empty())
	{
		std::string metadata = HttpRequest("api.spotify.com", "/v1/tracks/" + DecryptedSong.currentSong.substr(DecryptedSong.currentSong.find("spotify:track:") + 14), GetAccessToken(), true); // 14
		//Song name
		DecryptedSong.songName = strtok((char*)(metadata.substr(metadata.find("is_local") + 31)).c_str(), "\"");

		//Artist
		DecryptedSong.artist = strtok((char*)(metadata.substr(metadata.find("name") + 9)).c_str(), "\"");

		//Album
		std::string hmm = Utils::hex2string("68656967687422203A20363430");
		DecryptedSong.album = strtok((char*)(metadata.substr(metadata.find(hmm) + 404)).c_str(), "\"");

		//Cover
		DecryptedSong.coverart = strtok((char*)(metadata.substr(metadata.find("height") + 30)).c_str(), "\"");

		std::cout << "[Download finished]: " << DecryptedSong.songName << std::endl;
	}

	Utils::removeForbiddenChar(&DecryptedSong.songName);
	std::ofstream decryptedsongoutput(L".\\Songs\\" + Utils::ConvertUtf8ToUtf16(DecryptedSong.songName) + L".ogg", std::ios_base::binary);
	decryptedsongoutput.write((char*)DecryptedSong.outputData, DecryptedSong.decryptedtext_len);
	decryptedsongoutput.close();


	//AddTags(Utils::ConvertUtf8ToUtf16(DecryptedSong.songName), Utils::ConvertUtf8ToUtf16(DecryptedSong.artist), Utils::ConvertUtf8ToUtf16(DecryptedSong.album), Utils::ConvertUtf8ToUtf16(DecryptedSong.coverart));
	//Utils::deleteFile(L".\\XSongs\\" + Utils::ConvertUtf8ToUtf16(DecryptedSong.songName) + L".jpg");
}

void __declspec(naked) __fastcall Signal_stub(void* _this, DWORD edx, int a2, int a3)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push - 1
		push    117A780h
		push    Signal_back
		retn
	}
}

void __fastcall Signal_hk(void* _this, DWORD edx, int a2, int a3)
{
	check = true;
	Signal_stub(_this, edx, a2, a3);
}

void __declspec(naked) CmdAddText_stub(int a1, int a2, const char* fmt, const char* dummy0, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push    esi
		mov     esi, [ebp + 8]
		lea     eax, [ebp + 20]
		push    CmdAddText_back
		retn
	}
}

void CmdAddText_hk(int a1, int a2, const char* fmt, const char* dummy0, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5)
{
	if (fmt[8] == char(116) && fmt[9] == char(114) && fmt[10] == char(97) && fmt[11] == char(99) && fmt[12] == char(107) && fmt[13] == char(95) && fmt[14] == char(117) && fmt[15] == char(114) && fmt[16] == char(105))
	{
		if (dummy0[8] == char(97) && dummy0[9] == char(100)) //ad
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "[Ad detected]: " << dummy0 << std::endl;
			SetConsoleTextAttribute(hConsole, 10);
			__position = 29000;
		}
		else if (dummy0[8] == char(116) && dummy0[9] == char(114)) //tr
		{
			// std::cout << "Song: " << dummy0 << std::endl;

			std::string str(dummy0, strnlen(dummy0, 36));
			DecryptedSong.currentSong = str;
			std::string metadata = HttpRequest("api.spotify.com", "/v1/tracks/" + DecryptedSong.currentSong.substr(DecryptedSong.currentSong.find("spotify:track:") + 14), GetAccessToken(), true); // 14
			DecryptedSong.songName = strtok((char*)(metadata.substr(metadata.find("is_local") + 31)).c_str(), "\"");

			vars::lastSong = DecryptedSong.songName.c_str();
			vars::playing = true;
			__position = 0;
		}
	}

	CmdAddText_stub(a1, a2, fmt, dummy0, dummy1, dummy2, dummy3, dummy4, dummy5);
}

void __declspec(naked) GetFileID_stub(int* a1, int a2)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		mov     ecx, [ebp + 8]
		push    dword ptr[ebp + 12]
		push    GetFileID_back
		retn
	}
}

void CheckUserInputAndStartDownload()
{
	while (true)
	{
		if (vars::download)
		{
			std::thread t1(DownloadFileProcess);
			t1.detach();
			vars::download = false;
		}
		Sleep(100);
	}
}

void GetFileID_hk(int* a1, int a2)
{
	if (check)
	{
		EncryptedSong.fileID = (char*)(*(DWORD*)((*(DWORD*)(a1)) + 40));
		std::thread t2(CheckUserInputAndStartDownload);
		t2.detach();
		check = false;
	}

	GetFileID_stub(a1, a2);
}
void XPatchDownloads()
{
	Hook::InstallJmp(Signal, Signal_hk);
	//Hook::InstallJmp(CmdAddText, CmdAddText_hk2);
	Hook::InstallJmp(CmdAddText, CmdAddText_hk);
	Hook::InstallJmp(aes_set_encrypt_key, AES_set_encrypt_key_hk);
	Hook::InstallJmp(GetFileID, GetFileID_hk);
}