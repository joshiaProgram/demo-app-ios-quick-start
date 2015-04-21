//
//  RongUIKit.h
//  RongIMKit
//
//  Created by xugang on 15/1/13.
//  Copyright (c) 2015年 RongCloud. All rights reserved.
//



#ifndef __RongUIKit
#define __RongUIKit
#import <Foundation/Foundation.h>
#import <RongIMLib/RongIMLib.h>
#import "RCConversationViewController.h"

//dispatch message notification name
FOUNDATION_EXPORT NSString *const RCKitDispatchMessageNotification;

FOUNDATION_EXPORT NSString *const RCKitDispatchConnectionStatusChangedNotification;

@protocol RCIMUserInfoProvider <NSObject>

/**
 *  获取用户信息。
 *
 *  @param userId     用户 Id。
 *  @param completion 用户信息
 */
- (void)getUserInfoWithUserId:(NSString *)userId completion:(void(^)(RCUserInfo* userInfo))completion;
@end

@protocol RCIMGroupInfoProvider <NSObject>

/**
 *  获取群主信息.
 *
 *  @param groupId  群主ID.
 *  @param completion 获取完成调用的BLOCK.
 */

-(void)getGroupInfoWithGroupId:(NSString*)groupId completion:(void(^)(RCGroup* groupInfo))completion;

@end

/**
 @protocol RCIMReceiveMessageDelegate.
 接收消息的监听器。
 */
@protocol RCKitReceiveMessageDelegate <NSObject>

/**
 接收消息到消息后执行。
 
 @param message 接收到的消息。
 @param left    剩余消息数.
 */
-(void)onKitReceivedMessage:(RCMessage*)message left:(int)left;
@end

/**
 *  连接状态监听器，以获取连接相关状态。
 */
@protocol RCKitConnectionStatusChangeDelegate <NSObject>

/**
 *  网络状态变化。
 *
 *  @param status 网络状态。
 */
-(void)onKitConnectionStatusChanged:(RCConnectionStatus)status;
@end


@interface RongUIKit : NSObject

@property (nonatomic,strong) RCUserInfo* currentUserInfo;
@property (nonatomic,weak) id<RCIMUserInfoProvider> userInfoProvider;
@property (nonatomic,weak) id<RCIMGroupInfoProvider> groupInfoProvider;
@property (nonatomic,weak) id<RCKitReceiveMessageDelegate> receiveMessageListener;
@property (nonatomic,weak) id<RCKitConnectionStatusChangeDelegate> connectionChangedListener;
/**
 *  消息免打扰，默认是NO
 */
@property (nonatomic,assign) BOOL messageNotDisturb;
/**
 *  新消息提醒声音，默认YES开启，NO关闭.
 */
@property (nonatomic,assign) BOOL messageBeep;

/**
 获取界面组件的核心类单例。
 
 @return 界面组件的核心类单例。
 */
+(instancetype)sharedKit;

/**
 初始化 SDK。如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。
 
 @param appKey   从开发者平台申请的应用 appKey。
 @param deviceToken 用于 Apple Push Notification Service 的设备唯一标识。
 */
-(void)initWithAppKey:(NSString*)appKey deviceToken:(NSString*)deviceToken;


/**
 *  注册消息类型，如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。如果对消息类型进行扩展，可以忽略此方法。
 *
 *  @param messageClass   消息类型名称，对应的继承自 RCMessageContent 的消息类型。
 */

-(void)registerMessageType:(Class)messageClass;

/**
 建立连接，注意该方法回调在非调用线程，如果需要UI操作，请切换主线程，如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。
 
 @param token      从服务端获取的用户身份令牌（Token）。
 @param completion 调用完成的处理。
 @param error      调用返回的错误信息。
 */
-(void)connectWithToken:(NSString*)token
                success:(void (^)(NSString* userId))successBlock
                  error:(void (^)(RCConnectErrorCode status))errorBlock;


/**
 *  断开连接。如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。
 *
 *  @param isReceivePush 是否接收回调。
 */
-(void)disconnect:(BOOL)isReceivePush;

/**
 *  断开连接。如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。
 */
-(void)disconnect;

/**
 设置接收消息的监听器。如果使用IMKit，使用此方法，不再使用RongIMLib的同名方法。
 
 所有接收到的消息、通知、状态都经由此处设置的监听器处理。包括单聊消息、讨论组消息、群组消息、聊天室消息以及各种状态。<br>
 此处仅为扩展功能提供，默认可以不做实现。
 
 @param delegate 接收消息的监听器。
 */
-(void)setReceiveMessageDelegate:(id<RCKitReceiveMessageDelegate>)delegate;

/**
 *  监听网络连接
 *
 *  @param connectionChangedListener 监听器
 */
-(void)setConnectionChangedListener:(id<RCKitConnectionStatusChangeDelegate>)connectionChangedListener;

/**
 *  拨打VoIP电话
 *  @param targetId 对方userId
 */
- (void)startVoIPCallWithTargetId:(NSString *) targetId;

/**
 *  添加会话controller，维护会话生命周期。建议把会话添加到RongUIKit单例中管理生命周期，当会话中发送耗时消息的返回的时候，会话不会立即销毁。
 *
 *  @param controller 会话controller或者派生的会话controller
 */
-(void)addConversationController:(RCConversationViewController*)controller;
/**
 *  获取会话，维护会话生命周期。建议把会话添加到RongUIKit单例中管理生命周期，当再次进入会话的时候，可以从保存的会话中获取。
 *
 *  @param conversationType 会话类型
 *  @param targetId         targetId
 *
 *  @return 会话congtroller
 */
-(id)getConversationController:(RCConversationType)conversationType
                                                 targetId:(NSString*)targetId;
/**
 *  移除不活跃的会话，SDK会恰当时机自行调用，开发者不用主动调用。
 */
-(void)removeInactiveController;




@end


#endif