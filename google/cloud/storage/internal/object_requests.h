// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_REQUESTS_H_
#define GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_REQUESTS_H_

#include "google/cloud/storage/internal/generic_object_request.h"
#include "google/cloud/storage/internal/http_response.h"
#include "google/cloud/storage/object_metadata.h"
#include "google/cloud/storage/well_known_parameters.h"

namespace google {
namespace cloud {
namespace storage {
inline namespace STORAGE_CLIENT_NS {
namespace internal {
/**
 * Represents a request to the `Objects: list` API.
 */
class ListObjectsRequest
    : public GenericRequest<ListObjectsRequest, MaxResults, Prefix, Projection,
                            UserProject, Versions> {
 public:
  ListObjectsRequest() = default;
  explicit ListObjectsRequest(std::string bucket_name)
      : bucket_name_(std::move(bucket_name)) {}

  std::string const& bucket_name() const { return bucket_name_; }
  std::string const& page_token() const { return page_token_; }
  ListObjectsRequest& set_page_token(std::string page_token) {
    page_token_ = std::move(page_token);
    return *this;
  }

 private:
  std::string bucket_name_;
  std::string page_token_;
};

std::ostream& operator<<(std::ostream& os, ListObjectsRequest const& r);

struct ListObjectsResponse {
  static ListObjectsResponse FromHttpResponse(HttpResponse&& response);

  std::string next_page_token;
  std::vector<ObjectMetadata> items;
};

std::ostream& operator<<(std::ostream& os, ListObjectsResponse const& r);

/**
 * Represents a request to the `Objects: get` API.
 */
class GetObjectMetadataRequest
    : public GenericObjectRequest<
          GetObjectMetadataRequest, Generation, IfGenerationMatch,
          IfGenerationNotMatch, IfMetagenerationMatch, IfMetagenerationNotMatch,
          Projection, UserProject> {
 public:
  using GenericObjectRequest::GenericObjectRequest;
};

std::ostream& operator<<(std::ostream& os, GetObjectMetadataRequest const& r);

/**
 * Represents a request to the `Objects: insert` API with a string for the
 * media.
 *
 * This request type is used to upload objects with media that completely
 * fits in memory. Such requests are simpler than uploading objects streaming
 * objects.
 */
class InsertObjectMediaRequest
    : public GenericObjectRequest<InsertObjectMediaRequest, ContentEncoding,
                                  ContentType, EncryptionKey, IfGenerationMatch,
                                  IfGenerationNotMatch, IfMetagenerationMatch,
                                  IfMetagenerationNotMatch, KmsKeyName,
                                  PredefinedAcl, Projection, UserProject> {
 public:
  InsertObjectMediaRequest() : GenericObjectRequest(), contents_() {}

  explicit InsertObjectMediaRequest(std::string bucket_name,
                                    std::string object_name,
                                    std::string contents)
      : GenericObjectRequest(std::move(bucket_name), std::move(object_name)),
        contents_(std::move(contents)) {}

  std::string const& contents() const { return contents_; }

 private:
  std::string contents_;
};

std::ostream& operator<<(std::ostream& os, InsertObjectMediaRequest const& r);

/**
 * Represents a request to the `Objects: insert` API where the media will be
 * uploaded as a stream.
 *
 * This request type is used to upload objects where the media is not known in
 * advance, and it is uploaded using chunked encoding as it is generated by the
 * application.
 */
class InsertObjectStreamingRequest
    : public GenericObjectRequest<InsertObjectStreamingRequest, ContentEncoding,
                                  ContentType, EncryptionKey, IfGenerationMatch,
                                  IfGenerationNotMatch, IfMetagenerationMatch,
                                  IfMetagenerationNotMatch, KmsKeyName,
                                  PredefinedAcl, Projection, UserProject> {
 public:
  using GenericObjectRequest::GenericObjectRequest;
};

std::ostream& operator<<(std::ostream& os,
                         InsertObjectStreamingRequest const& r);

/**
 * Represents a request to the `Objects: copy` API.
 */
class CopyObjectRequest
    : public GenericRequest<
          CopyObjectRequest, DestinationPredefinedAcl, EncryptionKey,
          IfGenerationMatch, IfGenerationNotMatch, IfMetagenerationMatch,
          IfMetagenerationNotMatch, IfSourceGenerationMatch,
          IfSourceGenerationNotMatch, IfSourceMetagenerationMatch,
          IfSourceMetagenerationNotMatch, Projection, SourceGeneration,
          UserProject> {
 public:
  CopyObjectRequest() = default;
  CopyObjectRequest(std::string source_bucket, std::string source_object,
                    std::string destination_bucket,
                    std::string destination_object,
                    ObjectMetadata const& metadata)
      : source_bucket_(std::move(source_bucket)),
        source_object_(std::move(source_object)),
        destination_bucket_(std::move(destination_bucket)),
        destination_object_(std::move(destination_object)),
        json_payload_(metadata.JsonPayloadForCopy()) {}

  std::string const& source_bucket() const { return source_bucket_; }
  std::string const& source_object() const { return source_object_; }
  std::string const& destination_bucket() const { return destination_bucket_; }
  std::string const& destination_object() const { return destination_object_; }
  std::string const& json_payload() const { return json_payload_; }

 private:
  std::string source_bucket_;
  std::string source_object_;
  std::string destination_bucket_;
  std::string destination_object_;
  std::string json_payload_;
};

std::ostream& operator<<(std::ostream& os, CopyObjectRequest const& r);

/**
 * Represents a request to the `Objects: get` API with `alt=media`.
 */
class ReadObjectRangeRequest
    : public GenericObjectRequest<ReadObjectRangeRequest, EncryptionKey,
                                  Generation, IfGenerationMatch,
                                  IfGenerationNotMatch, IfMetagenerationMatch,
                                  IfMetagenerationNotMatch, UserProject> {
 public:
  ReadObjectRangeRequest() : GenericObjectRequest(), begin_(0), end_(0) {}

  // TODO(#724) - consider using StrongType<> for arguments with similar types.
  explicit ReadObjectRangeRequest(std::string bucket_name,
                                  std::string object_name, std::int64_t begin,
                                  std::int64_t end)
      : GenericObjectRequest(std::move(bucket_name), std::move(object_name)),
        begin_(begin),
        end_(end) {}

  // TODO(#724) - consider using StrongType<> for arguments with similar types.
  explicit ReadObjectRangeRequest(std::string bucket_name,
                                  std::string object_name)
      : GenericObjectRequest(std::move(bucket_name), std::move(object_name)),
        begin_(0),
        end_(0) {}

  std::int64_t begin() const { return begin_; }
  std::int64_t end() const { return end_; }

 private:
  std::int64_t begin_;
  std::int64_t end_;
};

std::ostream& operator<<(std::ostream& os, ReadObjectRangeRequest const& r);

struct ReadObjectRangeResponse {
  std::string contents;
  std::int64_t first_byte;
  std::int64_t last_byte;
  std::int64_t object_size;

  static ReadObjectRangeResponse FromHttpResponse(HttpResponse&& response);
};

std::ostream& operator<<(std::ostream& os, ReadObjectRangeResponse const& r);

/**
 * Represents a request to the `Objects: delete` API.
 */
class DeleteObjectRequest
    : public GenericObjectRequest<DeleteObjectRequest, Generation,
                                  IfGenerationMatch, IfGenerationNotMatch,
                                  IfMetagenerationMatch,
                                  IfMetagenerationNotMatch, UserProject> {
 public:
  using GenericObjectRequest::GenericObjectRequest;
};

std::ostream& operator<<(std::ostream& os, DeleteObjectRequest const& r);

/**
 * Represents a request to the `Objects: update` API.
 */
class UpdateObjectRequest
    : public GenericObjectRequest<
          UpdateObjectRequest, Generation, IfGenerationMatch,
          IfGenerationNotMatch, IfMetagenerationMatch, IfMetagenerationNotMatch,
          PredefinedAcl, Projection, UserProject> {
 public:
  UpdateObjectRequest() = default;
  explicit UpdateObjectRequest(std::string bucket_name, std::string object_name,
                               ObjectMetadata metadata)
      : GenericObjectRequest(std::move(bucket_name), std::move(object_name)),
        metadata_(std::move(metadata)) {}

  /// Returns the request as the JSON API payload.
  std::string json_payload() const { return metadata_.JsonPayloadForUpdate(); }

  ObjectMetadata const& metadata() const { return metadata_; }

 private:
  ObjectMetadata metadata_;
};

std::ostream& operator<<(std::ostream& os, UpdateObjectRequest const& r);

/**
 * Represents a request to the `Objects: compose` API.
 */
class ComposeObjectRequest
    : public GenericObjectRequest<ComposeObjectRequest, EncryptionKey,
                                  Generation, DestinationPredefinedAcl,
                                  KmsKeyName, IfGenerationMatch,
                                  IfMetagenerationMatch, UserProject> {
 public:
  ComposeObjectRequest() = default;
  explicit ComposeObjectRequest(
      std::string bucket_name,
      std::vector<ComposeSourceObject> const& source_objects,
      std::string destination_object_name,
      ObjectMetadata destination_object_metadata);

  /// Returns the request as the JSON API payload.
  std::string json_payload() const { return json_payload_; }

  ObjectMetadata const& destination_metadata() const {
    return destination_metadata_;
  }

 private:
  ObjectMetadata destination_metadata_;
  std::string json_payload_;
};

std::ostream& operator<<(std::ostream& os, ComposeObjectRequest const& r);

/**
 * Represents a request to the `Buckets: patch` API.
 */
class PatchObjectRequest
    : public GenericObjectRequest<
          PatchObjectRequest, IfMetagenerationMatch, IfMetagenerationNotMatch,
          PredefinedAcl, PredefinedDefaultObjectAcl, Projection, UserProject> {
 public:
  PatchObjectRequest() = default;
  explicit PatchObjectRequest(std::string bucket_name, std::string object_name,
                              ObjectMetadata const& original,
                              ObjectMetadata const& updated);
  explicit PatchObjectRequest(std::string bucket_name, std::string object_name,
                              ObjectMetadataPatchBuilder const& patch);

  std::string const& payload() const { return payload_; }

 private:
  std::string payload_;
};

std::ostream& operator<<(std::ostream& os, PatchObjectRequest const& r);

/**
 * Represents a request to the `Objects: rewrite` API.
 */
class RewriteObjectRequest
    : public GenericRequest<
          RewriteObjectRequest, DestinationKmsKeyName, DestinationPredefinedAcl,
          EncryptionKey, IfGenerationMatch, IfGenerationNotMatch,
          IfMetagenerationMatch, IfMetagenerationNotMatch,
          IfSourceGenerationMatch, IfSourceGenerationNotMatch,
          IfSourceMetagenerationMatch, IfSourceMetagenerationNotMatch,
          Projection, SourceGeneration, UserProject> {
 public:
  RewriteObjectRequest() = default;
  RewriteObjectRequest(std::string source_bucket, std::string source_object,
                       std::string destination_bucket,
                       std::string destination_object,
                       std::string rewrite_token,
                       ObjectMetadata const& metadata)
      : source_bucket_(std::move(source_bucket)),
        source_object_(std::move(source_object)),
        destination_bucket_(std::move(destination_bucket)),
        destination_object_(std::move(destination_object)),
        rewrite_token_(std::move(rewrite_token)),
        json_payload_(metadata.JsonPayloadForCopy()) {}

  std::string const& source_bucket() const { return source_bucket_; }
  std::string const& source_object() const { return source_object_; }
  std::string const& destination_bucket() const { return destination_bucket_; }
  std::string const& destination_object() const { return destination_object_; }
  std::string const& rewrite_token() const { return rewrite_token_; }
  std::string const& json_payload() const { return json_payload_; }

 private:
  std::string source_bucket_;
  std::string source_object_;
  std::string destination_bucket_;
  std::string destination_object_;
  std::string rewrite_token_;
  std::string json_payload_;
};

std::ostream& operator<<(std::ostream& os, RewriteObjectRequest const& r);

/// Holds an `Objects: rewrite` response.
struct RewriteObjectResponse {
  static RewriteObjectResponse FromHttpResponse(HttpResponse const& response);

  std::uint64_t total_bytes_rewritten;
  std::uint64_t object_size;
  bool done;
  std::string rewrite_token;
  ObjectMetadata resource;
};

std::ostream& operator<<(std::ostream& os, RewriteObjectResponse const& r);

}  // namespace internal
}  // namespace STORAGE_CLIENT_NS
}  // namespace storage
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_GOOGLE_CLOUD_STORAGE_INTERNAL_OBJECT_REQUESTS_H_