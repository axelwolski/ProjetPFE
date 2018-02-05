// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OedivXuej.h"
#include "Engine/GameInstance.h"
#include "NetworkGameInstance.generated.h"

/**
*
*/
UCLASS()
class OEDIVXUEJ_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// ***** create session *****
	UNetworkGameInstance(const FObjectInitializer& ObjectInitializer);
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	// Delegate called when session created 
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	// Handles to registered delegates for creating/starting a session 
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;


	// ***** find session *****
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	void OnFindSessionsComplete(bool bWasSuccessful);

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	// ***** join session *****
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	// ***** destroy session *****
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	// ***** blueprints ******
	UFUNCTION(BlueprintCallable, Category = "Network")
		void StartOnlineGame();
	UFUNCTION(BlueprintCallable, Category = "Network")
		void FindOnlineGames();
	UFUNCTION(BlueprintCallable, Category = "Network")
		void JoinOnlineGame();
	UFUNCTION(BlueprintCallable, Category = "Network")
		void DestroySessionAndLeaveGame();

private:
	bool bSearchDone; //a search needs to be done before tring to join 


					  // ***** TESTS *****
	bool bTestCreateActivated;
	bool bTestJoinActivated;
	bool bTestCreateOtherSession;
	bool bTestFindSessionAgain;
	UFUNCTION(BlueprintCallable, Category = "Network")
		void SessionCreationTests();
	UFUNCTION(BlueprintCallable, Category = "Network")
		void SessionJoinTests();
};
