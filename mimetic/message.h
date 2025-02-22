/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: message.h,v 1.13 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_MESSAGE_H_
#define _MIMETIC_MESSAGE_H_
#include <time.h>
#include <sys/types.h>
#include <mimetic/libconfig.h>
#include <mimetic/mimeentity.h>
#include <mimetic/utils.h>
#include <mimetic/codec/codec.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

namespace mimetic
{

class ContentType;

/// Base class for text/* MIME entities.
struct TextEntity: public MimeEntity
{
    /**
     *  Sets its content-type to "text/unknown"
     */
    static std::shared_ptr<TextEntity> create();
    static std::shared_ptr<TextEntity> create(const std::string& text);
    /**
     *  Sets its content-type to "text/unknown",
     *  sets its body with the "text" variable and adds
     *  "charset=us-ascii" content-type parameter
     */
    static std::shared_ptr<TextEntity> create(const std::string& text, const std::string& charset);

protected:
    TextEntity();
    explicit TextEntity(const std::string& text);
    TextEntity(const std::string& text, const std::string& charset);
};

/// text/plain entity class
/*!
    TextPlain is a MimeEntity that defaults its ContentType to "text/plain" 
    and its charset to "us-ascii".
 */
struct TextPlain: public TextEntity
{
    static std::shared_ptr<TextPlain> create(const std::string& text);
    /**
     * constructs a TextPlain object, assigns <i>text</i> to its body and adds
     * a ContentType::Param("charset", <i>charset</i>) to ContentType parameter list
     */
    static std::shared_ptr<TextPlain> create(const std::string& text, const std::string& charset);

protected:
    explicit TextPlain(const std::string& text);
    TextPlain(const std::string& text, const std::string& charset);
};


/// text/enriched entity class
struct TextEnriched: public TextEntity
{
    static std::shared_ptr<TextEnriched> create(const std::string& text);
    /**
     * constructs a TextPlain object, assigns <i>text</i> to its body and adds
     * a ContentType::Param("charset", <i>charset</i>) to ContentType parameter list
     */
    static std::shared_ptr<TextEnriched> create(const std::string& text, const std::string& charset);

protected:
    explicit TextEnriched(const std::string& text);
    TextEnriched(const std::string& text, const std::string& charset);
};

/// Base multipart/* class
/**
   Base multipart/ * class. Its constructor sets the content-type
   to "multipart/unknown" and adds and fills a "boundary" parameter
 */
struct MultipartEntity: public MimeEntity
{
    static std::shared_ptr<MultipartEntity> create();

protected:
    MultipartEntity();
};

/// multipart/mixed entity class
struct MultipartMixed: public MultipartEntity
{
    static std::shared_ptr<MultipartMixed> create();

protected:
    MultipartMixed();
};

/// multipart/parallel entity class
struct MultipartParallel: public MultipartEntity
{
    static std::shared_ptr<MultipartParallel> create();

protected:
    MultipartParallel();
};

/// multipart/alternative entity class
struct MultipartAlternative: public MultipartEntity
{
    static std::shared_ptr<MultipartAlternative> create();

protected:
    MultipartAlternative();
};

/// multipart/digest entity class
struct MultipartDigest: public MultipartEntity
{
    static std::shared_ptr<MultipartDigest> create();

protected:
    MultipartDigest();
};


/// application/octet-stream entity class
struct ApplicationOctStream: public MimeEntity
{
    static std::shared_ptr<ApplicationOctStream> create();
    template<typename Codec>
    static std::shared_ptr<ApplicationOctStream> create(const std::string&, const Codec& c=Base64::Encoder());

    std::string type() const;
    void type(const std::string&);
    uint padding() const;
    void padding(unsigned int);
    bool operator()() const { return isValid(); }
    bool isValid() const { return m_status; }

protected:
    ApplicationOctStream();
    template<typename Codec>
    ApplicationOctStream(const std::string&, const Codec& c);

    std::string m_fqn;
    bool m_status;
};

/// Helper class to embed file attachments
struct Attachment: public MimeEntity
{
    /**
     * defaults to application/octet-stream
     */
    static std::shared_ptr<Attachment> create(const std::string&);
    static std::shared_ptr<Attachment> create(const std::string&, const ContentType&);
    template<typename Codec>
    static std::shared_ptr<Attachment> create(const std::string&, const Codec& c );
    template<typename Codec>
    static std::shared_ptr<Attachment> create(const std::string&, const ContentType&, const Codec& c);

    bool operator()() const { return isValid(); }
    bool isValid() const { return m_status; }

protected:
    explicit Attachment(const std::string&);
    Attachment(const std::string&, const ContentType&);
    template<typename Codec>
    Attachment(const std::string&, const Codec& c);
    template<typename Codec>
    Attachment(const std::string&, const ContentType&, const Codec& c);

private:
    template<typename Codec>
    void set(const std::string&, const ContentType&, const Codec& c);
    std::string m_fqn;
    bool m_status;
};

/// image/jpeg attachment
struct ImageJpeg: public Attachment
{
    static std::shared_ptr<ImageJpeg> create(const std::string& fqn);
    template<typename Codec>
    static std::shared_ptr<ImageJpeg> create(const std::string& fqn, const Codec& c);

protected:
    explicit ImageJpeg(const std::string& fqn)
    : Attachment(fqn, ContentType("image","jpeg"))
    {
    }
    template<typename Codec>
    ImageJpeg(const std::string& fqn, const Codec& c)
    : Attachment(fqn, ContentType("image","jpeg"), c)
    {
    }
};

/// audio/basic attachment
struct AudioBasic: public Attachment
{
    static std::shared_ptr<AudioBasic> create(const std::string& fqn);
    template<typename Codec>
    static std::shared_ptr<AudioBasic> create(const std::string& fqn, const Codec& c);

protected:
    explicit AudioBasic(const std::string& fqn)
    : Attachment(fqn, ContentType("audio","basic"))
    {
    }
    template<typename Codec>
    AudioBasic(const std::string& fqn, const Codec& c)
    : Attachment(fqn, ContentType("audio","basic"), c)
    {
    }
};



/// message/rfc822 entity type
struct MessageRfc822: public MimeEntity
{
    static std::shared_ptr<MessageRfc822> create(const std::shared_ptr<MimeEntity>&);

protected:
    explicit MessageRfc822(const std::shared_ptr<MimeEntity>&);

protected:
    std::ostream& write(std::ostream&,const char*) const;
private:
    std::shared_ptr<MimeEntity> m_me;
};



/**
 * defaults to application/octet-stream
 */
template<typename Codec>
Attachment::Attachment(const std::string& fqn, const Codec& codec)
{
    set(fqn, ContentType("application","octet-stream"), codec);
}

template<typename Codec>
Attachment::Attachment(const std::string& fqn, const ContentType& ctype, const Codec& codec)
{
    set(fqn, ctype, codec);
}

template<typename Codec>
std::shared_ptr<Attachment> Attachment::create(const std::string& fqn, const Codec& codec) {
    return init(std::shared_ptr<Attachment>(new Attachment(fqn, codec)));
}

template<typename Codec>
std::shared_ptr<Attachment> Attachment::create(const std::string& fqn, const ContentType& ctype, const Codec& codec) {
    return init(std::shared_ptr<Attachment>(new Attachment(fqn, ctype, codec)));
}

template<typename Codec>
void Attachment::set(const std::string& fqn, const ContentType& ctype, const Codec& codec)
{
    Header& h = header();
    m_fqn = fqn;
    m_status = false;
    std::string filename = utils::extractFilename(m_fqn);
    // Content-Type
    h.contentType(ctype);
    h.contentType().paramList().push_back(ContentType::Param("name", filename));
    
    // Content-Transfer-Encoding
    h.contentTransferEncoding().mechanism(codec.name());

    // Content-Disposition
    h.contentDisposition().type("attachment");
    h.contentDisposition().paramList().push_back(ContentDisposition::Param("filename", filename));
    
    m_status = body().load(m_fqn, codec);
}

    
template<typename Codec>
ApplicationOctStream::ApplicationOctStream(const std::string& fqn, const Codec& codec)
{
    Header& h = header();
    m_fqn = fqn;
    m_status = false;
    std::string filename = utils::extractFilename(m_fqn);
    // Content-Type
    h.contentType(ContentType("application", "octet-stream"));
    h.contentType().paramList().push_back(ContentType::Param("name", filename));
    
    // Content-Transfer-Encoding
    h.contentTransferEncoding().mechanism(codec.name());

    // Content-Disposition
    h.contentDisposition().type("attachment");
    h.contentDisposition().paramList().push_back(ContentDisposition::Param("filename", filename));
    
    m_status = body().load(m_fqn, codec);
}

template<typename Codec>
std::shared_ptr<ApplicationOctStream> ApplicationOctStream::create(const std::string& fqn, const Codec& codec)
{
    return init(std::shared_ptr<ApplicationOctStream>(new ApplicationOctStream(fqn, codec)));
}

template<typename Codec>
std::shared_ptr<ImageJpeg> ImageJpeg::create(const std::string& fqn, const Codec& codec)
{
    return init(std::shared_ptr<ImageJpeg>(new ImageJpeg(fqn, codec)));
}

template<typename Codec>
std::shared_ptr<AudioBasic> AudioBasic::create(const std::string& fqn, const Codec& codec)
{
    return init(std::shared_ptr<AudioBasic>(new AudioBasic(fqn, codec)));
}

}


#endif

