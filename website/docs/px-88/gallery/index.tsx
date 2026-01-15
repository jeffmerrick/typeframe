import Image from "@theme/IdealImage";
import Layout from "@theme/Layout";
import Heading from "@theme/Heading";
import styles from "./gallery.module.css";
import { ReactNode } from "react";

const GalleryGrid = ({ images }) => {
  return (
    <div className={styles.galleryGrid}>
      {images.map((ideal, i) => {
        const largest = ideal.src.images.reduce(
          (max, img) => (img.width > max.width ? img : max),
          ideal.src.images[0]
        );
        return (
          <a
            href={largest.path}
            target="_blank"
            rel="noopener noreferrer"
            key={i}
          >
            <Image img={ideal} />
          </a>
        );
      })}
    </div>
  );
};

export default function Gallery(): ReactNode {
  return (
    <Layout title="Photo Gallery">
      <div className="container padding-vert--lg">
        <Heading as="h1">Photo Gallery</Heading>
        <Heading as="h2">Typeframe PX-88</Heading>
        <GalleryGrid
          images={[
            require("../../../docs/px-88/img/typeframe-px-88-01.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-02.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-03.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-04.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-05.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-06.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-07.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-08.jpg"),
          ]}
        />
        <Heading as="h3">Earlier Prototype with Alternate Keycaps</Heading>
        <GalleryGrid
          images={[
            require("../../../docs/px-88/img/typeframe-px-88-10.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-09.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-11.jpg"),
          ]}
        />
        <hr />
        <Heading as="h1">Community Builds</Heading>
        <Heading as="h3">Paul's Build</Heading>
        <p>
          <a href="https://biosrhythm.com/" target="_blank">
            Paul Rickards
          </a>{" "}
          built a Typeframe with a ton of enhancements including a Pi 5,
          speaker, alternate power switch, and modified case to fit a different
          keyboard. He even made a battery status system tray icon! Thanks to
          Paul for also helping to uncover some issues with the build and
          documentation.
          <br />
          <a
            href="https://mastodon.social/@paulrickards/115703267825409926"
            target="_blank"
          >
            Check out his build on Mastodon
          </a>
          .
        </p>
        <GalleryGrid
          images={[
            require("../../../docs/px-88/img/typeframe-px-88-paul-1.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-paul-2.jpg"),
            require("../../../docs/px-88/img/typeframe-px-88-paul-3.jpg"),
          ]}
        />
        <hr />

        <p>
          Did you build your own?{" "}
          <a href="mailto:jeff@typeframe.net">E-mail me</a> photos and I'll add
          them to the gallery!
        </p>
      </div>
    </Layout>
  );
}
