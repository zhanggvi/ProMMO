// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Online.h"
#include "Base64.h"
#include "math.h"
//#include "Online.h"
#include "OnlineFriendsInterface.h"
#include "OnlineTournamentsInterface.h"
#include "ILoginFlowManager.h"
#include "MyBasePickup.h"
#include "MyBaseVendor.h"
#include "GameplayAbilitySpec.h"
//#include "VictoryBPFunctionLibrary.h"
#include "MyPlayerController.generated.h"

// 4.16 login flow thing
#ifndef UETOPIA_SUBSYSTEM
#define UETOPIA_SUBSYSTEM FName(TEXT("UEtopia"))
#endif

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendsChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendInviteReceivedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFriendInviteReceivedUETopiaDisplayUIDelegate, FString, SenderUserTitle, FString, SenderUserKeyId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecentPlayersChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyJoinedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyInviteReceivedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyInviteReceivedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartyInviteResponseReceivedUETopiaDisplayUIDelegate, FString, SenderUserTitle, FString, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyDataReceivedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatChannelsChangedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChatRoomMessageReceivedDisplayUIDelegate, FString, SenderUserKeyId, FString, ChatMessage, FString, chatRoomId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatPrivateMessageReceivedDisplayUIDelegate, FString, SenderUserKeyId, FString, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentListChangedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentDataReadUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSellStartedDelegate, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVendorInteractDisplayUIDelegate, AMyBaseVendor*, VendorRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVendorInfoCompleteDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVendorInteractChangedDelegate, FString, VendorKeyId, bool, bIsInteracting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitiesChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityInterfaceChangedDelegate);

USTRUCT(BlueprintType)
struct FMyFriend {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsOnline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsPlayingThisGame;
};

USTRUCT(BlueprintType)
struct FMyFriends {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyFriend> MyFriends;
};

USTRUCT(BlueprintType)
struct FMyRecentPlayer {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerTitle;
};

USTRUCT(BlueprintType)
struct FMyRecentPlayers {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyRecentPlayer> MyRecentPlayers;
};

USTRUCT(BlueprintType)
struct FMyPartyInvitation {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString partyTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString partyKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString senderUserTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString senderUserKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsInvited;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsInviteAccepted;
};

USTRUCT(BlueprintType)
struct FMyPartyInvitations {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyPartyInvitation> MyPartyInvitations;
};

USTRUCT(BlueprintType)
struct FMyChatChannel {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString chatChannelKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString chatChannelTitle;
};

USTRUCT(BlueprintType)
struct FMyChatChannels {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyChatChannel> MyChatChannels;
};


/** Tournaments have a heirarchy of data:
* Tournament List
* -- Tournament
* ---- Tournament Configuration
* ---- Tournament Team List
* ------ Tournament Team
* ---- Tournament Round List
* ------ Tournament Round
* -------- Tournament Round Match List
* ---------- Tournament Round Match
*/

USTRUCT(BlueprintType)
struct FMyTournamentRoundMatch {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Status;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team1Title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team1KeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team1Winner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team1Loser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team2Title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team2KeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team2Winner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team2Loser;

};

USTRUCT(BlueprintType)
struct FMyTournamentRound {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 RoundIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentRoundMatch> RoundMatchList;

};

USTRUCT(BlueprintType)
struct FMyTournamentTeam {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString KeyId;


};


USTRUCT(BlueprintType)
struct FMyTournament {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 tournamentId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString tournamentKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString tournamentTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsGroupTournament;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString groupTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString PrizeDistributionType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString region;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString GameMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 donationAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerBuyIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentTeam> TeamList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentRound> RoundList;

};


/**
* Inventory contains the commonly used variables, as well as Inventory "stats" that can be customized for eash game without modifying the cpp code
*/


USTRUCT(BlueprintType)
struct FMyInventoryItemStat {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemStatKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 itemStatValue;

};

USTRUCT(BlueprintType)
struct FMyInventorySlot {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
		int32 quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	bool bCanBeUsed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	FText UseText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	bool bCanBeStacked;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	int32 MaxStackSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<float> Attributes;
};


USTRUCT(BlueprintType)
struct FMyVendorItem {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
		int32 quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 PricePerUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString buyingOfferExpires;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool buyingOffer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool selling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool claimable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<float> Attributes;
};



USTRUCT(BlueprintType)
struct FMyGrantedAbility {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		struct FGameplayAbilitySpecHandle AbilityHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString classPath; // used to reference this ability from the Ability Slot.  TODO maybe do this differently.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString description;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TSubclassOf<UGameplayAbility> Ability;
};

USTRUCT(BlueprintType)
struct FMyAbilitySlot {
	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString description;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString classTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString classPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bCanBeUsed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FText UseText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FKey Key;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsValid; // true is a ability slot that is in use.  False is empty.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FMyGrantedAbility GrantedAbility;
};

/**
* Info associated with a party identifier
* THis is a duplicate that is also declared in the UEtopia plugin
* I could not get the include to work, so I'm putting this here for now
* TODO delete this and import it properly.
*/
class FOnlinePartyIdUEtopiaDup :
	public FOnlinePartyId
{
public:
	FOnlinePartyIdUEtopiaDup(const FString& InKeyId)
		: key_id(InKeyId)
	{
	}
	//~FOnlinePartyId() {};
	const uint8* GetBytes() const override;
	int32 GetSize() const override;
	bool IsValid() const override;
	FString ToString() const override;
	FString ToDebugString() const override;
private:
	const FString key_id;
};

/**
*
*/
UCLASS()
class PROMMO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	//Constructor
	AMyPlayerController();

	// Load from path
	static FORCEINLINE UClass* LoadClassFromPath(const FString& Path)
	{
		if (Path == "") return NULL;

		return StaticLoadClass(UObject::StaticClass(), NULL, *Path, NULL, LOAD_None, NULL);
	}

	//Get Path
	static FORCEINLINE FString GetClassPath(UClass* ClassPtr)
	{
		return FStringClassReference(ClassPtr).ToString();
	}


public:

	///////////////////////////////////////////////////////////
	// This section from
	// https://wiki.unrealengine.com/UMG,_Referencing_UMG_Widgets_in_Code

	// Note: that am using forward declaration Because am not including the
	// widget in the header and to prevent circular dependency.
	// you dont need to do that if you include the Widget Class in the .h
	// forward declaration is just putting "class" before the class name so the compiler know its a
	// class but its not included in the header and don't freak out. Ex. �class UUserWidget�

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMainMenu;

	// Variable to hold the widget After Creating it.
	UUserWidget* MyMainMenu;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	
	///////////////////////////////////////////////////////////


	// Deprecated - Unused.
	// Use the login function inside the LoginFlow Widget Instead.
	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	//	void Login();

	// Unused in Most MMOs
	// This can be used if servers should have a lobby before gameplay begins.
	UFUNCTION(BlueprintCallable, Category = "UETOPIA", Server, Reliable, WithValidation)
		void RequestBeginPlay();

	FString PlayerUniqueNetId;

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void TravelPlayer(const FString& ServerUrl);

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 Experience; // total experience all time

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 ExperienceThisLevel;  // Experience so far in this level

	// Keep track of the state of this player's persistent data
	// Prevent the case of a user disconnecting before the data has been populated and leading to a data wipe.
	bool PlayerDataLoaded;



	///////////////////
	// Friend Functions
	///////////////////

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void InviteUserToFriends(const FString& UserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RejectFriendInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AcceptFriendInvite(const FString& senderUserKeyId);

	//////////////////////////
	// Party Related Functions
	//////////////////////////

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateParty(const FString& PartyTitle, const FString& TournamentKeyId, bool TournamentParty);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RejectPartyInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void InviteUserToParty(const FString& PartyKeyId, const FString& UserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AcceptPartyInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void LeaveParty(const FString& PartyKeyId);

	/////////////////////////
	// Chat related Functions
	/////////////////////////

	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RefreshChatChannelList(const FUniqueNetId& LocalUserId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateChatChannel(const FString& ChatChannelTitle);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void JoinChatChannel(const FString& ChatChannelTitle);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void SendChatMessage(int32 CurrentChannelIndex, FString ChatMessage);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void ExitChatChannel(int32 CurrentChannelIndex);

	///////////////////////////////
	// TOURNAMENT related Functions
	///////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void FetchJoinableTournaments();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateTournament(const FString& TournamentTitle, const FString& GameMode, const FString& Region, int32 TeamMin, int32 TeamMax, int32 donationAmount, int32 playerBuyIn);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void ReadTournamentDetails(const FString& TournamentKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void JoinTournament(const FString& TournamentKeyId);

	//////////////////////////////
	// Inventory related functions
	//////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool IsSlotEmpty(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		int32 SearchEmptySlot();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		int32 SearchSlotWithAvailableSpace(AMyBasePickup* ClassTypeIn);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool AddItem( AMyBasePickup* ClassTypeIn, int32 quantity, bool checkOnly);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool RemoveItemAtIndex( int32 index, int32 quantity);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptRemoveItemAtIndex(int32 index, int32 quantity);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool SwapSlots(int32 index1, int32 index2);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptSwapSlots(int32 index1, int32 index2);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool SplitStack(int32 index, int32 quantity);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptSplitStack(int32 index, int32 quantity);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool SplitStackToIndex(int32 indexFrom, int32 indexTo, int32 quantity);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptSplitStackToIndex(int32 indexFrom, int32 indexTo, int32 quantity);

	// This is the blueprint function used to drop an item.
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool DropItem(int32 index);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptDropItem(int32 index);

	// This is the blueprint function used to drop an item.
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool UseItem(int32 index);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptUseItem(int32 index);

	// This is the blueprint function used to drag a stackable item on top of another stack.
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool AddToIndex(int32 indexFrom, int32 indexTo);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptAddToIndex(int32 indexFrom, int32 indexTo);

	///////////////////////////
	// VENDOR RELATED FUNCTIONS
	///////////////////////////

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptSetVendorInteract(AMyBaseVendor* VendorRef);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AttemptVendorInteract(AMyBaseVendor* VendorRef);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptVendorUpdate(const FString& Title, const FString& Description, bool bIsSelling, bool bIsBuying, bool bDisableForPickup);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AttemptVendorUpdate(const FString& Title, const FString& Description, bool bIsSelling, bool bIsBuying, bool bDisableForPickup);

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UETOPIA")
		bool bInteractingWithVendor;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "UETOPIA")
		FString InteractingWithVendorKeyId;

	// This is the blueprint function used to add an item into a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool AddItemToVendor(int32 indexFrom, int32 quantity, int32 pricePerUnit, const FString& vendorKeyId);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptAddItemToVendor(int32 indexFrom, int32 quantity, int32 pricePerUnit, const FString& vendorKeyId);

	// This is the blueprint function used to add an item into a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool RemoveItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	// This is the blueprint function used to add an item into a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool BuyItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId, int32 quantity);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBuyItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId, int32 quantity);

	// This is the blueprint function used to withdraw currency from a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool WithdrawFromVendor(const FString& vendorKeyId);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerWithdrawFromVendor(const FString& vendorKeyId);

	// This is the blueprint function used to add an item into a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool DeclineOfferFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerDeclineOfferFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	// This is the blueprint function used to add an item into a vendor
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool ClaimItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerClaimItemFromVendor(const FString& vendorKeyId, const FString& vendorItemKeyId);

	////////////////////////////
	// Ability Related Functions
	////////////////////////////

	// This is the blueprint function used to clear an ability slot
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool RemoveAbilityFromBar(int32 index);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptRemoveAbilityFromBar(int32 index);

	// This is the blueprint function used to add an ability to a slot
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool AddAbilityToBar(int32 index, FMyGrantedAbility GrantedAbility);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptAddAbilityToBar(int32 index, FMyGrantedAbility GrantedAbility);

	// This is the blueprint function used to swap ability slots
	// It will ultimately perform the function on the server, and replicate the data back to the client(s)
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool SwapAbilityBarLocations(int32 indexFrom, int32 indexTo);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptSwapAbilityBarLocations(int32 index, int32 indexTo);

	/////////////////////////////////////////
	// HTTP calls which are performed on client
	/////////////////////////////////////////

	FString APIURL;
	bool PerformJsonHttpRequest(void(AMyPlayerController::*delegateCallback)(FHttpRequestPtr, FHttpResponsePtr, bool), FString APIURI, FString ArgumentString);


	// Server allocate
	// This should be a part of the OSS at some point.
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool ServerAllocate();
	void ServerAllocateComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);



	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	bool GetVendorInfo(FString vendorKeyId);
	void GetVendorInfoComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	
	// DELEGATES

	// This is the delegate to grab on to in the UI
	// When it fires, it signals that you should refresh the friends list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnFriendsChangedDelegate OnFriendsChanged;

	// when this fires, the OSS has received a friend invite.
	//UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	//	FOnFriendInviteReceivedUETopiaDelegate OnFriendInviteReceived;

	// when this fires, display the friend invitation UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnFriendInviteReceivedUETopiaDisplayUIDelegate OnFriendInviteReceivedDisplayUI;

	// When this fires, it signals that you should refresh the recent player's list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnRecentPlayersChangedDelegate OnRecentPlayersChanged;
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnPartyJoinedUETopiaDelegate OnPartyJoined;
	// when this fires, the OSS has received a party invite.
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteReceivedUETopiaDelegate OnPartyInviteReceived;

	// when this fires, display the party invitation UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteReceivedUETopiaDisplayUIDelegate OnPartyInviteReceivedDisplayUI;

	// when this fires, display the party invitation response UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteResponseReceivedUETopiaDisplayUIDelegate OnPartyInviteResponseReceivedDisplayUI;

	// when this fires, update the party UI widget
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyDataReceivedUETopiaDisplayUIDelegate OnPartyDataReceivedUETopiaDisplayUI;

	// when this fires, update the Chat Channel List
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatChannelsChangedUETopiaDelegate OnChatChannelsChangedUETopia;

	// when this fires, add a chat room message to the chat list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatRoomMessageReceivedDisplayUIDelegate OnChatRoomMessageReceivedDisplayUIDelegate;

	// And this one is a private or system chat - not coming from a room.
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatPrivateMessageReceivedDisplayUIDelegate OnChatPrivateMessageReceivedDisplayUIDelegate;

	// when this fires, refresh the tournament UI list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnTournamentListChangedUETopiaDisplayUIDelegate OnTournamentListChangedUETopiaDisplayUIDelegate;

	// when this fires, refresh the tournament details panel
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnTournamentDataReadUETopiaDisplayUIDelegate OnTournamentDataReadUETopiaDisplayUIDelegate;

	// This is the delegate to grab on to in the UI
	// When it fires, it signals that you should refresh the inventory
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnInventoryChangedDelegate OnInventoryChanged;

	

	// Delegate which signals that a user has clicked the sell button in the inventory interface.  Vendors respond by populating the sell dialog
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnInventoryItemSellStartedDelegate FOnInventoryItemSellStarted;

	// this just fires the delegate
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	void InventoryItemSellStart(int32 index);

	// Delegate which signals that a vendor interaction event has occurred, and we need to display the Vendor UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnVendorInteractDisplayUIDelegate FOnVendorInteractDisplayUI;

	// Delegate which signals that a vendor Infor request has finished, and we need to update the vendor UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnVendorInfoCompleteDelegate FOnVendorInfoComplete;

	// This is the delegate to grab on to in the UI
	// When it fires, it signals that you should refresh the abilities
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnAbilitiesChangedDelegate OnAbilitiesChanged;

	
	// This is the delegate to grab on to in the UI
	// When it fires, it signals that you should refresh the hotbar
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnAbilityInterfaceChangedDelegate OnAbilityInterfaceChanged;

	// Rep notify to tell the client to load the new inventory
	//UPROPERTY(ReplicatedUsing = OnRep_OnAbilityInterfaceChange)
	//	bool DoRep_AbilityInterfaceChanged;

	

	// Delegate which is triggered whenever a vendor interaction starts or ends
	//UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	//	FOnVendorInteractChangedDelegate FOnVendorInteractChanged;

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void InventoryItemSellConfirm(int32 index, int32 pricePerUnit);

	// This is the function that gets called on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttemptInventoryItemSellConfirm(int32 index, int32 pricePerUnit);

	// Handle to our external delegates
	// This is bound into the online subsystem.

	FOnReadFriendsListComplete OnReadFriendsListCompleteDelegate;
	FOnQueryRecentPlayersComplete OnQueryRecentPlayersCompleteDelegate;
	FOnCreatePartyComplete OnCreatePartyCompleteDelegate;
	FOnSendPartyInvitationComplete OnSendPartyInvitationComplete;
	//FOnPartyJoined FOnPartyJoinedUETopiaDelegate;
	FOnPartyInviteReceived OnPartyInviteReceivedUEtopiaDelegate;
	//FOnPartyInviteResponseReceived OnPartyInviteResponseReceivedComplete;
	//FOnChatRoomMessageReceivedDisplayUIDelegate OnChatRoomMessageReceivedDisplayUIDelegate;

	FOnCreateTournamentComplete OnCreateTournamentCompleteDelegate;

	// CACHED DATA FOR PASSING IN TO UI

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyFriend> MyCachedFriends;
	//FMyFriends MyCachedFriends;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	FMyRecentPlayers MyCachedRecentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyPartyInvitation> MyCachedPartyInvitations;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyFriend> MyCachedPartyMembers;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	FMyChatChannels MyCachedChatChannels;

	// tournament cache
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournament> MyCachedTournaments;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FMyTournament MyCachedTournament;

	// General Party Data

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyPartyTitle;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyPartyKeyId;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		int32 MyPartyMaxMemberCount;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		int32 MyPartyCurrentMemberCount;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		bool IAmCaptain;

	// Inventory

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OnInventoryChange, Category = "UETOPIA")
		TArray<FMyInventorySlot> InventorySlots;

	// Rep notify to tell the client to load the new inventory
	//UPROPERTY(ReplicatedUsing = OnRep_OnInventoryChange)
	//	bool DoRep_InventoryChanged;

	// this function just calls the delegate.
	UFUNCTION(Client, Reliable)
		void OnRep_OnInventoryChange();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 InventoryCapacity;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "UETOPIA")
	//	TArray<FMyAbilitySlot> AbilitySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 AbilityCapacity;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UETOPIA")
		int32 CurrencyAvailable;

	// Vendors 
	// We cache the currently in-use vendor's information locally on each client.  Requests for purchases or sales go through the server.
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyCurrentVendorTitle;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyCurrentVendorDescription;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyCurrentVendorKeyId;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		int32 MyCurrentVendorCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		bool MyCurrentVendorIsOwnedByMe;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UETOPIA")
		AMyBaseVendor* MyCurrentVendorRef;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyVendorItem> MyCurrentVendorInventory;

	// Abilities
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_OnAbilityInterfaceChange, Category = "UETOPIA")
		TArray<FMyAbilitySlot> MyAbilitySlots;

	UFUNCTION(Client, Reliable)
		void OnRep_OnAbilityInterfaceChange();

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "UETOPIA")
		int32 AbilitySlotsPerRow;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_OnAbilitiesChange, Category = "UETOPIA")
		TArray<FMyGrantedAbility> MyGrantedAbilities;

	// Rep notify to tell the client to load the new inventory
	//UPROPERTY(ReplicatedUsing = OnRep_OnAbilitiesChange)
	//	bool DoRep_AbilitiesChanged;

	// this function activates after replication
	UFUNCTION(Client, Reliable)
		void OnRep_OnAbilitiesChange();

	ILoginFlowManager::FOnPopupDismissed OnPopupDismissedUEtopiaDelegate;
private:

	// 4.16 login flow stuff - moving to loginwidget
	//void FOnPopupDismissed(const TSharedRef<SWidget>& DisplayWidget);
	//ILoginFlowManager::FOnPopupDismissed OnDisplayLoginWidget(const TSharedRef<SWidget>& DisplayWidget);
	//void OnDismissLoginWidget();
	//FReply CancelLoginFlow();

	TSharedPtr<ILoginFlowManager> LoginFlowManager;

	//TSharedPtr<class SWebBrowser> WebBrowserWidget;

	TSharedPtr<SWidget> DisplayWidgetRef;


	// This is fired by the Online subsystem after it polls the friend list.
	void OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	// This function is called by a delegate whenever the friends list changes.
	// The purpose of this is to emit a delegate which we can grab onto in the UI
	void OnFriendsChange();

	// THis is called when the OSS sees a friend request
	void OnFriendInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FUniqueNetId& SenderId);

	// This is fired by the Online subsystem after it polls the recent players list.
	//(FOnQueryRecentPlayersComplete, const FUniqueNetId& /*UserId*/, const FString& /*Namespace*/, bool /*bWasSuccessful*/, const FString& /*Error*/);
	//UFUNCTION()
	void OnReadRecentPlayersComplete(const FUniqueNetId& UserId, const FString& ListName, bool bWasSuccessful, const FString& ErrorStr);

	// This function is called by a delegate whenever the RecentPlayers list changes.
	// The purpose of this is to emit a delegate which we can grab onto in the UI
	void OnRecentPlayersChange();

	// This is fired by the Online subsystem after it Creates a party.
	void OnCreatePartyComplete(const FUniqueNetId& UserId, const TSharedPtr<const FOnlinePartyId>&, const ECreatePartyCompletionResult);

	void OnPartyJoinedComplete(const FUniqueNetId& UserId, const FOnlinePartyId& PartyId);

	void OnPartyInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId);

	void OnPartyInviteResponseReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId, const EInvitationResponse Response);

	// This is fired by the online subsystem when it receives new party data
	// DECLARE_MULTICAST_DELEGATE_ThreeParams(F_PREFIX(OnPartyDataReceived), const FUniqueNetId& /*LocalUserId*/, const FOnlinePartyId& /*PartyId*/, const TSharedRef<FOnlinePartyData>& /*PartyData*/);

	void OnPartyDataReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const TSharedRef<FOnlinePartyData>& PartyData);

	// THis is fired by the online subsystem when it receives notification that the user's chat room list has changed.
	// ADD THIS TO /Engine/Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineChatInterface.h
	// DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatRoomListChanged, const FUniqueNetId& /*UserId*/, const FString& /*Error*/);
	void OnChatRoomListReadComplete(const FUniqueNetId& LocalUserId, const FString& ErrorStr);

	// This is fired by the online subsystem when it receives a new chat message coming from a room
	//DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChatRoomMessageReceived, const FUniqueNetId& /*UserId*/, const FChatRoomId& /*RoomId*/, const TSharedRef<FChatMessage>& /*ChatMessage*/);
	void OnChatRoomMessageReceivedComplete(const FUniqueNetId& SenderUserId, const FChatRoomId& RoomId, const TSharedRef<FChatMessage>& ChatMessage);

	// THis one is a private/system chat - not originating from a room
	//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatPrivateMessageReceived, const FUniqueNetId& /*UserId*/, const TSharedRef<FChatMessage>& /*ChatMessage*/);
	void OnChatPrivateMessageReceivedComplete(const FUniqueNetId& SenderUserId, const TSharedRef<FChatMessage>& ChatMessage);

	// Tournament stuff
	//DECLARE_MULTICAST_DELEGATE_OneParam(F_PREFIX(OnTournamentListDataChanged), const FUniqueNetId& /*LocalUserId*/);
	void OnTournamentListDataChanged(const FUniqueNetId& LocalUserId);

	// The subsystem has finished reading the tournament details.  Copy them into our local struct and trigger the delegate to update the UI
	void OnTournamentDetailsReadComplete();


	TArray<TSharedRef<IOnlinePartyJoinInfo>> PendingInvitesArray;


};