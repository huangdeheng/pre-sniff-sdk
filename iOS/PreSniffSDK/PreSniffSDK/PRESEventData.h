#import "PRESDomain.h"

@interface PRESEventData : PRESDomain <NSCoding>

@property (nonatomic, copy, readonly) NSString *envelopeTypeName;
@property (nonatomic, copy, readonly) NSString *dataTypeName;
@property (nonatomic, strong) NSDictionary *measurements;

@end
