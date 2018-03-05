// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"

#define TEST 1

UNetworkGameInstance::UNetworkGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bSearchDone(false), bTestCreateActivated(false), bTestCreateOtherSession(false), bTestJoinActivated(false), bMultiPlayerGame(false), MaxNumPlayers(4) {
	// Binding de delegates
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNetworkGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UNetworkGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetworkGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNetworkGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNetworkGameInstance::OnDestroySessionComplete);
}

void UNetworkGameInstance::StartOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, true, 4);
}

void UNetworkGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId(), true, true);
	bSearchDone = true;
}

void UNetworkGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FOnlineSessionSearchResult SearchResult;

	if (bSearchDone && SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				SearchResult = SessionSearch->SearchResults[i];
				JoinSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
#if TEST
	if (SessionSearch->SearchResults.Num() == 0 && bTestCreateActivated) {
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 4/5 : No session to join OK")));
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 5/5 : Destroying session")));
		DestroySessionAndLeaveGame();
	}
#endif
}

void UNetworkGameInstance::DestroySessionAndLeaveGame() {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

bool UNetworkGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//Fill in all the Session Settings
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UNetworkGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
#if TEST
			if (!bWasSuccessful && bTestCreateOtherSession) {
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 2/5 : Can't create other session OK")));
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 3/5 : Searching sessions")));
				FindOnlineGames();
			}
#endif
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Sessions->StartSession(SessionName);

#if TEST
				if (bTestCreateActivated) {
					GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 1/5 : Session creation OK")));
					GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 2/5 : Creating other session")));
					bTestCreateOtherSession = true;
					StartOnlineGame();
				}
#endif
			}
		}

	}
}

void UNetworkGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Arena_V1", true, "listen");
	}
}

void UNetworkGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void UNetworkGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
#if TEST
				if (bTestJoinActivated) {
					if (bTestFindSessionAgain) {
						GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 4/4 : Session still exist OK")));
						bTestJoinActivated = false;
						bTestFindSessionAgain = false;
					}
					else {
						GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 1/5 : Session found OK")));
						GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 2/5 : Trying to join Session")));
						JoinOnlineGame();
					}
				}
#endif
			}
#if TEST
			if (bTestCreateActivated && SessionSearch->SearchResults.Num() == 0) {
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 3/5 : Can't find own session OK")));
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 4/5 : Trying to join no session")));
				JoinOnlineGame();
			}
#endif
		}
	}
}

bool UNetworkGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UNetworkGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController * const PlayerController = GetFirstLocalPlayerController();
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
#if TEST
			if (bTestJoinActivated) {
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 2/4 : Session joined OK")));
				GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 3/4 : Try to quit session")));
				DestroySessionAndLeaveGame();
			}
#endif
		}
	}
}

void UNetworkGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "Arena_V1", true);
#if TEST
				if (bTestCreateActivated) {
					GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 5/5 : Session destroyed OK")));
					bTestCreateActivated = false;
					bTestCreateOtherSession = false;
				}
				if (bTestJoinActivated) {
					GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 3/4 : Session quit OK")));
					GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 4/4 : Session still exist?")));
					bTestFindSessionAgain = true;
					FindOnlineGames();
				}
#endif
			}
		}
	}
}


// *************************************************** TESTS ************************************************


void UNetworkGameInstance::SessionCreationTests()
{
#if TEST
	bTestCreateActivated = true;
	StartOnlineGame();
#endif
}

void UNetworkGameInstance::SessionJoinTests()
{
#if TEST
	bTestJoinActivated = true;
	GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Blue, FString::Printf(TEXT("TEST 1/5 : Trying to Find the opened session")));
	FindOnlineGames();
#endif
}
